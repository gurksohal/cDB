//
// Created by G on 11/21/2024.
//

#ifndef CDB_PROJECTPLAN_H
#define CDB_PROJECTPLAN_H

#include "Plan.h"

class ProjectPlan : public Plan {
public:
    ProjectPlan(std::shared_ptr<Plan> p, std::vector<std::string>& fld_list);
    auto open() -> std::shared_ptr<Scan> override;
    auto blocksAccessed() -> int override;
    auto recordsOutput() -> int override;
    auto distinctValues() -> int override;
    auto schema() -> Schema override;

private:
    std::shared_ptr<Plan> p;
    Schema sch {};
};

#endif  // CDB_PROJECTPLAN_H
