//
// Created by G on 11/23/2024.
//

#ifndef CDB_INDEXSELECTPLAN_H
#define CDB_INDEXSELECTPLAN_H

#include "../../metadata/IndexInfo.h"
#include "../../plan/Plan.h"

class IndexSelectPlan : public Plan {
public:
    IndexSelectPlan(std::shared_ptr<Plan> p, IndexInfo* ii, std::shared_ptr<Constant> val);
    auto open() -> std::shared_ptr<Scan> override;
    auto blocksAccessed() -> int override;
    auto recordsOutput() -> int override;
    auto distinctValues() -> int override;
    auto schema() -> Schema override;

private:
    std::shared_ptr<Plan> p;
    IndexInfo* ii;
    std::shared_ptr<Constant> val;
};

#endif  // CDB_INDEXSELECTPLAN_H
