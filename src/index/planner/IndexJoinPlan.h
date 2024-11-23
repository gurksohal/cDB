//
// Created by G on 11/23/2024.
//

#ifndef CDB_INDEXJOINPLAN_H
#define CDB_INDEXJOINPLAN_H

#include "../../metadata/IndexInfo.h"
#include "../../plan/Plan.h"

class IndexJoinPlan : public Plan {
public:
    IndexJoinPlan(std::shared_ptr<Plan> p1, std::shared_ptr<Plan> p2, IndexInfo* ii, std::string join_field);
    auto open() -> std::shared_ptr<Scan> override;
    auto blocksAccessed() -> int override;
    auto recordsOutput() -> int override;
    auto distinctValues() -> int override;
    auto schema() -> Schema override;

private:
    std::shared_ptr<Plan> p1;
    std::shared_ptr<Plan> p2;
    IndexInfo* ii;
    std::string join_field;
    Schema sch;
};

#endif  // CDB_INDEXJOINPLAN_H
