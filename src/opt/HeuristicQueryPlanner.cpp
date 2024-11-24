//
// Created by G on 11/24/2024.
//

#include "HeuristicQueryPlanner.h"

#include "../plan/ProjectPlan.h"

HeuristicQueryPlanner::HeuristicQueryPlanner(MetadataMgr* mdm) : mdm(mdm) {}

auto HeuristicQueryPlanner::createPlan(QueryData data, Transaction& tx) -> std::shared_ptr<Plan> {
    for (auto& tbl_name : data.tables()) {
        table_planners.emplace_back(tbl_name, data.pred(), &tx, mdm);
    }

    auto current_plan = getLowestSelectPlan();
    while (!table_planners.empty()) {
        auto p = getLowestJoinPlan(current_plan);
        if (p) {
            current_plan = p;
        } else {
            current_plan = getLowestProductPlan(current_plan);
        }
    }

    auto s = data.fields();
    return std::make_shared<ProjectPlan>(current_plan, s);
}

auto HeuristicQueryPlanner::getLowestSelectPlan() -> std::shared_ptr<Plan> {
    TablePlanner* best_tp = nullptr;
    std::shared_ptr<Plan> best_plan;

    for (auto& tp : table_planners) {
        auto plan = tp.makeSelectPlan();
        if (!best_plan || plan->recordsOutput() < best_plan->recordsOutput()) {
            best_tp = &tp;
            best_plan = plan;
        }
    }

    if (best_plan) {
        table_planners.erase(std::remove_if(table_planners.begin(), table_planners.end(),
                                            [&](const TablePlanner& tp) { return &tp == best_tp; }),
                             table_planners.end());
    }

    return best_plan;
}

auto HeuristicQueryPlanner::getLowestJoinPlan(const std::shared_ptr<Plan>& current) -> std::shared_ptr<Plan> {
    TablePlanner* best_tp = nullptr;
    std::shared_ptr<Plan> best_plan;

    for (auto& tp : table_planners) {
        auto plan = tp.makeJoinPlan(current);
        if (plan && (best_tp == nullptr || plan->recordsOutput() < best_plan->recordsOutput())) {
            best_tp = &tp;
            best_plan = plan;
        }
    }

    if (best_plan) {
        table_planners.erase(std::remove_if(table_planners.begin(), table_planners.end(),
                                            [&](const TablePlanner& tp) { return &tp == best_tp; }),
                             table_planners.end());
    }

    return best_plan;
}

auto HeuristicQueryPlanner::getLowestProductPlan(const std::shared_ptr<Plan>& current) -> std::shared_ptr<Plan> {
    TablePlanner* best_tp = nullptr;
    std::shared_ptr<Plan> best_plan;

    for (auto& tp : table_planners) {
        auto plan = tp.makeProductPlan(current);
        if (best_plan == nullptr || plan->recordsOutput() < best_plan->recordsOutput()) {
            best_tp = &tp;
            best_plan = plan;
        }
    }

    if (best_plan) {
        table_planners.erase(std::remove_if(table_planners.begin(), table_planners.end(),
                                            [&](const TablePlanner& tp) { return &tp == best_tp; }),
                             table_planners.end());
    }

    return best_plan;
}
