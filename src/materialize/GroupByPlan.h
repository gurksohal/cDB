//
// Created by G on 11/23/2024.
//

#ifndef CDB_GROUPBYPLAN_H
#define CDB_GROUPBYPLAN_H

#include "../plan/Plan.h"
#include "../tx/Transaction.h"
#include "AggregationFn.h"

class GroupByPlan : public Plan {
public:
    GroupByPlan(Transaction& tx, std::shared_ptr<Plan> p, std::vector<std::string> group_fields,
                std::vector<std::shared_ptr<AggregationFn>> agg_fns);
    auto open() -> std::shared_ptr<Scan> override;
    auto blocksAccessed() -> int override;
    auto recordsOutput() -> int override;
    auto distinctValues() -> int override;
    auto schema() -> Schema override;

private:
    std::shared_ptr<Plan> p;
    std::vector<std::string> group_fields;
    std::vector<std::shared_ptr<AggregationFn>> agg_fns;
    Schema sch;
};

#endif  // CDB_GROUPBYPLAN_H
