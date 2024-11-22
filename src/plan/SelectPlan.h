//
// Created by G on 11/21/2024.
//

#ifndef CDB_SELECTPLAN_H
#define CDB_SELECTPLAN_H

#include "../query/Predicate.h"
#include "Plan.h"

class SelectPlan : public Plan {
public:
    SelectPlan(std::shared_ptr<Plan> p, Predicate pred);
    auto open() -> std::shared_ptr<Scan> override;
    auto blocksAccessed() -> int override;
    auto recordsOutput() -> int override;
    auto distinctValues() -> int override;
    auto schema() -> Schema override;

private:
    std::shared_ptr<Plan> p;
    Predicate pred;
};

#endif  // CDB_SELECTPLAN_H
