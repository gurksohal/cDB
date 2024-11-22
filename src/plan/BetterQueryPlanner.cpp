//
// Created by G on 11/22/2024.
//

#include "BetterQueryPlanner.h"

#include "../parse/Parser.h"
#include "ProductPlan.h"
#include "ProjectPlan.h"
#include "SelectPlan.h"
#include "TablePlan.h"

BetterQueryPlanner::BetterQueryPlanner(MetadataMgr* mdm) : mdm(mdm) {}

auto BetterQueryPlanner::createPlan(QueryData data, Transaction& tx) -> std::shared_ptr<Plan> {  // NOLINT
    std::vector<std::shared_ptr<Plan>> plans;
    for (const auto& tbl_name : data.tables()) {
        std::string view_def = mdm->getViewDef(tbl_name, tx);
        if (!view_def.empty()) {
            Parser parser(view_def);
            plans.push_back(createPlan(parser.query(), tx));
        } else {
            plans.push_back(std::make_shared<TablePlan>(&tx, tbl_name, mdm));
        }
    }

    auto p = plans.front();
    plans.erase(plans.begin());
    for (const auto& next_plan : plans) {
        auto choice1 = std::make_shared<ProductPlan>(next_plan, p);
        auto choice2 = std::make_shared<ProductPlan>(p, next_plan);
        if (choice1->blocksAccessed() < choice2->blocksAccessed()) {
            p = choice1;
        } else {
            p = choice2;
        }
    }

    p = std::make_shared<SelectPlan>(p, data.pred());
    auto t = data.fields();
    p = std::make_shared<ProjectPlan>(p, t);

    return p;
}
