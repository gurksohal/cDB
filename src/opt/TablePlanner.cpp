//
// Created by G on 11/24/2024.
//

#include "TablePlanner.h"

#include <utility>

#include "../index/planner/IndexJoinPlan.h"
#include "../index/planner/IndexSelectPlan.h"
#include "../multibuffer/MultiBufferProductPlan.h"
#include "../plan/SelectPlan.h"

TablePlanner::TablePlanner(const std::string& tbl_name, Predicate my_pred, Transaction* tx, MetadataMgr* mdm)
    : my_pred(std::move(my_pred)),
      tx(tx),
      my_plan(tx, tbl_name, mdm),
      my_schema(my_plan.schema()),
      indexes(mdm->getIndexInfo(tbl_name, *tx)) {}

auto TablePlanner::makeSelectPlan() -> std::shared_ptr<Plan> {
    auto p = makeIndexSelect();
    if (!p) {
        p = std::make_shared<TablePlan>(my_plan);
    }
    return addSelectPred(p);
}

auto TablePlanner::makeJoinPlan(const std::shared_ptr<Plan>& current) -> std::shared_ptr<Plan> {
    auto curr_sch = current->schema();
    auto join_pred = my_pred.joinSubPred(my_schema, curr_sch);
    if (!join_pred) {
        return {};
    }
    auto p = makeIndexJoin(current, curr_sch);
    if (!p) {
        p = makeProductJoin(current, curr_sch);
    }
    return p;
}

auto TablePlanner::makeProductPlan(const std::shared_ptr<Plan>& current) -> std::shared_ptr<Plan> {
    auto p = addSelectPred(std::make_shared<TablePlan>(my_plan));
    return std::make_shared<MultiBufferProductPlan>(tx, current, p);
}

auto TablePlanner::makeIndexSelect() -> std::shared_ptr<Plan> {
    for (auto& entry : indexes) {
        auto fld_name = entry.first;
        auto val = my_pred.equatesWithConstant(fld_name);
        if (val) {
            auto ii = indexes[fld_name];
            return std::make_shared<IndexSelectPlan>(std::make_shared<TablePlan>(my_plan), &ii,
                                                     std::make_shared<Constant>(*val));
        }
    }

    return {};
}

auto TablePlanner::makeIndexJoin(const std::shared_ptr<Plan>& current, Schema& curr_sch) -> std::shared_ptr<Plan> {
    for (auto& entry : indexes) {
        auto fld_name = entry.first;
        auto outer_field = my_pred.equatesWithField(fld_name);
        if (!outer_field.empty() && curr_sch.hasField(outer_field)) {
            auto ii = indexes[fld_name];
            auto p = std::make_shared<IndexJoinPlan>(current, std::make_shared<TablePlan>(my_plan), &ii, outer_field);
            return addJoinPred(p, curr_sch);
        }
    }
    return {};
}

auto TablePlanner::makeProductJoin(const std::shared_ptr<Plan>& current, Schema& curr_sch) -> std::shared_ptr<Plan> {
    auto p = makeProductPlan(current);
    return addJoinPred(p, curr_sch);
}

auto TablePlanner::addSelectPred(std::shared_ptr<Plan> p) -> std::shared_ptr<Plan> {
    auto select_pred = my_pred.selectSubPred(my_schema);
    if (select_pred) {
        return std::make_shared<SelectPlan>(p, *select_pred);
    }
    return p;
}

auto TablePlanner::addJoinPred(std::shared_ptr<Plan> p, Schema& curr_sch) -> std::shared_ptr<Plan> {
    auto join_pred = my_pred.joinSubPred(curr_sch, my_schema);
    if (join_pred) {
        return std::make_shared<SelectPlan>(p, *join_pred);
    }
    return p;
}