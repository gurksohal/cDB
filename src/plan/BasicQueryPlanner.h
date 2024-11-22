//
// Created by G on 11/21/2024.
//

#ifndef CDB_BASICQUERYPLANNER_H
#define CDB_BASICQUERYPLANNER_H

#include "QueryPlanner.h"

class BasicQueryPlanner : public QueryPlanner {
public:
    explicit BasicQueryPlanner(MetadataMgr* mdm);
    auto createPlan(QueryData data, Transaction& tx) -> std::shared_ptr<Plan> override;

private:
    MetadataMgr* mdm;
};

#endif  // CDB_BASICQUERYPLANNER_H
