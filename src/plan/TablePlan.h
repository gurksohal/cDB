//
// Created by G on 11/21/2024.
//

#ifndef CDB_TABLEPLAN_H
#define CDB_TABLEPLAN_H

#include <string>

#include "../metadata/MetadataMgr.h"
#include "../metadata/StatInfo.h"
#include "../record/Layout.h"
#include "../tx/Transaction.h"
#include "Plan.h"

class TablePlan : public Plan {
public:
    TablePlan(Transaction* tx, const std::string& tbl_name, MetadataMgr* md);
    auto open() -> std::shared_ptr<Scan> override;
    auto blocksAccessed() -> int override;
    auto recordsOutput() -> int override;
    auto distinctValues() -> int override;
    auto schema() -> Schema override;

private:
    std::string tbl_name;
    Transaction* tx;
    Layout layout;
    StatInfo si;
};

#endif  // CDB_TABLEPLAN_H
