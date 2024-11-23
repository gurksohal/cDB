//
// Created by G on 11/24/2024.
//

#ifndef CDB_MERGEJOINPLAN_H
#define CDB_MERGEJOINPLAN_H

#include "../plan/Plan.h"
#include "../tx/Transaction.h"

class MergeJoinPlan : public Plan {
public:
    MergeJoinPlan(Transaction& tx, std::shared_ptr<Plan> p1, std::shared_ptr<Plan> p2, std::string fld_name1,
                  std::string fld_name2);
    auto open() -> std::shared_ptr<Scan> override;
    auto blocksAccessed() -> int override;
    auto recordsOutput() -> int override;
    auto distinctValues() -> int override;
    auto schema() -> Schema override;

private:
    std::shared_ptr<Plan> p1;
    std::shared_ptr<Plan> p2;
    std::string fld_name1;
    std::string fld_name2;
    Schema sch;
};

#endif  // CDB_MERGEJOINPLAN_H
