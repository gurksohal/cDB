//
// Created by G on 11/21/2024.
//

#ifndef CDB_PLAN_H
#define CDB_PLAN_H

#include <memory>

#include "../query/Scan.h"
#include "../record/Schema.h"

class Plan {
public:
    Plan() = default;
    Plan(Plan&&) = default;
    Plan(const Plan&) = default;
    auto operator=(const Plan&) -> Plan& = default;
    auto operator=(Plan&&) -> Plan& = default;
    virtual ~Plan() = default;
    virtual auto open() -> std::shared_ptr<Scan> = 0;
    virtual auto blocksAccessed() -> int = 0;
    virtual auto recordsOutput() -> int = 0;
    virtual auto distinctValues() -> int = 0;
    virtual auto schema() -> Schema = 0;
};
#endif  // CDB_PLAN_H
