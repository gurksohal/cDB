//
// Created by G on 11/23/2024.
//

#ifndef CDB_MATERIALIZEPLAN_H
#define CDB_MATERIALIZEPLAN_H

#include "../plan/Plan.h"
#include "../tx/Transaction.h"

class MaterializePlan : Plan {
public:
    MaterializePlan(Transaction* tx, std::shared_ptr<Plan> src_plan);
    auto open() -> std::shared_ptr<Scan> override;
    auto blocksAccessed() -> int override;
    auto recordsOutput() -> int override;
    auto distinctValues() -> int override;
    auto schema() -> Schema override;

private:
    std::shared_ptr<Plan> src_plan;
    Transaction* tx;
};

#endif  // CDB_MATERIALIZEPLAN_H
