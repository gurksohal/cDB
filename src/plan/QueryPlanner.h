//
// Created by G on 11/21/2024.
//

#ifndef CDB_QUERYPLANNER_H
#define CDB_QUERYPLANNER_H

#include "../metadata/MetadataMgr.h"
#include "../parse/QueryData.h"
#include "../tx/Transaction.h"
#include "Plan.h"

class QueryPlanner {
public:
    QueryPlanner() = default;
    QueryPlanner(QueryPlanner&) = default;
    QueryPlanner(QueryPlanner&&) = default;
    auto operator=(const QueryPlanner&) -> QueryPlanner& = default;
    auto operator=(QueryPlanner&&) -> QueryPlanner& = default;
    virtual auto createPlan(QueryData data, Transaction& tx) -> std::shared_ptr<Plan> = 0;
    virtual ~QueryPlanner() = default;
};
#endif  // CDB_QUERYPLANNER_H
