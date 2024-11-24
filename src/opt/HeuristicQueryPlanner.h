//
// Created by G on 11/24/2024.
//

#ifndef CDB_HEURISTICQUERYPLANNER_H
#define CDB_HEURISTICQUERYPLANNER_H

#include "../plan/QueryPlanner.h"
#include "TablePlanner.h"

class HeuristicQueryPlanner : public QueryPlanner {
public:
    explicit HeuristicQueryPlanner(MetadataMgr* mdm);

private:
public:
    auto createPlan(QueryData data, Transaction& tx) -> std::shared_ptr<Plan> override;

private:
    auto getLowestSelectPlan() -> std::shared_ptr<Plan>;
    auto getLowestJoinPlan(const std::shared_ptr<Plan>& current) -> std::shared_ptr<Plan>;
    auto getLowestProductPlan(const std::shared_ptr<Plan>& current) -> std::shared_ptr<Plan>;
    std::vector<TablePlanner> table_planners;
    MetadataMgr* mdm;
};

#endif  // CDB_HEURISTICQUERYPLANNER_H
