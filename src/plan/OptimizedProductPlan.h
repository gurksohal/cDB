//
// Created by G on 11/21/2024.
//

#ifndef CDB_OPTIMIZEDPRODUCTPLAN_H
#define CDB_OPTIMIZEDPRODUCTPLAN_H

#include "Plan.h"

class OptimizedProductPlan : public Plan {
public:
    OptimizedProductPlan(const std::shared_ptr<Plan>& p1, const std::shared_ptr<Plan>& p2);
    auto open() -> std::shared_ptr<Scan> override;
    auto blocksAccessed() -> int override;
    auto recordsOutput() -> int override;
    auto distinctValues() -> int override;
    auto schema() -> Schema override;

private:
    std::shared_ptr<Plan> best_plan;
};

#endif  // CDB_OPTIMIZEDPRODUCTPLAN_H
