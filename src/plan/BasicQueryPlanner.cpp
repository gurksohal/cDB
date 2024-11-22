//
// Created by G on 11/21/2024.
//

#include "BasicQueryPlanner.h"

#include "../parse/Parser.h"
#include "ProductPlan.h"
#include "ProjectPlan.h"
#include "SelectPlan.h"
#include "TablePlan.h"

BasicQueryPlanner::BasicQueryPlanner(MetadataMgr* mdm) : mdm(mdm) {}

auto BasicQueryPlanner::createPlan(QueryData data, Transaction& tx) -> std::shared_ptr<Plan> {  // NOLINT
    std::vector<std::shared_ptr<Plan>> plans;
    for (auto& tbl_name : data.tables()) {
        auto view_def = mdm->getViewDef(tbl_name, tx);
        if (!view_def.empty()) {
            Parser parser(view_def);
            plans.push_back(createPlan(parser.query(), tx));
        } else {
            plans.push_back(std::make_shared<TablePlan>(&tx, tbl_name, mdm));
        }
    }

    std::shared_ptr<Plan> p = plans.front();
    plans.erase(plans.begin());
    for (const auto& next_plan : plans) {
        p = std::make_shared<ProductPlan>(p, next_plan);
    }

    p = std::make_shared<SelectPlan>(p, data.pred());
    auto t = data.fields();
    p = std::make_shared<ProjectPlan>(p, t);
    return p;
}
