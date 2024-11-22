//
// Created by G on 11/21/2024.
//

#ifndef CDB_PRODUCTPLAN_H
#define CDB_PRODUCTPLAN_H

#include "Plan.h"

class ProductPlan : public Plan {
public:
    ProductPlan(const std::shared_ptr<Plan>& p1, const std::shared_ptr<Plan>& p2);
    auto open() -> std::shared_ptr<Scan> override;
    auto blocksAccessed() -> int override;
    auto recordsOutput() -> int override;
    auto distinctValues() -> int override;
    auto schema() -> Schema override;

private:
    std::shared_ptr<Plan> p1;
    std::shared_ptr<Plan> p2;
    Schema schema_m {};
};

#endif  // CDB_PRODUCTPLAN_H
