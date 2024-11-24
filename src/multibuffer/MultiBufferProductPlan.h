//
// Created by G on 11/24/2024.
//

#ifndef CDB_MULTIBUFFERPRODUCTPLAN_H
#define CDB_MULTIBUFFERPRODUCTPLAN_H

#include "../materialize/TempTable.h"
#include "../plan/Plan.h"
#include "../tx/Transaction.h"

class MultiBufferProductPlan : public Plan {
public:
    MultiBufferProductPlan(Transaction* tx, std::shared_ptr<Plan> lhs, std::shared_ptr<Plan> rhs);
    auto open() -> std::shared_ptr<Scan> override;
    auto blocksAccessed() -> int override;
    auto recordsOutput() -> int override;
    auto distinctValues() -> int override;
    auto schema() -> Schema override;

private:
    auto copyRecordsFrom(Plan* p) -> std::unique_ptr<TempTable>;
    Transaction* tx;
    std::shared_ptr<Plan> lhs;
    std::shared_ptr<Plan> rhs;
    Schema schema_m;
};

#endif  // CDB_MULTIBUFFERPRODUCTPLAN_H
