//
// Created by G on 11/15/2024.
//

#ifndef CDB_INDEXINFO_H
#define CDB_INDEXINFO_H

#include <string>

#include "../record/Layout.h"
#include "../record/Schema.h"
#include "../tx/Transaction.h"
#include "StatInfo.h"

class IndexInfo {
public:
    IndexInfo() = default;
    IndexInfo(std::string name, std::string fld, Schema tbl_schema, Transaction* tx, StatInfo si);
    auto open() -> void;  // TODO: return Index
    auto blocksAccessed() -> int;
    auto recordsOutput() -> int;
    auto distinctValues(const std::string& fld) -> int;

private:
    auto createIdxLayout() -> Layout;
    std::string idx_name;
    std::string fld_name;
    Transaction* tx {};
    Schema tbl_schema;
    Layout idx_layout;
    StatInfo si {};
};

#endif  // CDB_INDEXINFO_H
