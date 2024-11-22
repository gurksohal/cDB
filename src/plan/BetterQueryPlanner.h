//
// Created by G on 11/22/2024.
//

#ifndef CDB_BETTERQUERYPLANNER_H
#define CDB_BETTERQUERYPLANNER_H

#include "QueryPlanner.h"

class BetterQueryPlanner : public QueryPlanner {
public:
    explicit BetterQueryPlanner(MetadataMgr* mdm);
    auto createPlan(QueryData data, Transaction& tx) -> std::shared_ptr<Plan> override;

private:
    MetadataMgr* mdm;
};

#endif  // CDB_BETTERQUERYPLANNER_H
