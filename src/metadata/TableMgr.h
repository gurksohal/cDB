//
// Created by G on 11/15/2024.
//

#ifndef CDB_TABLEMGR_H
#define CDB_TABLEMGR_H

#include "../record/Layout.h"
#include "../tx/Transaction.h"

class TableMgr {
public:
    static int const MAX_NAME = 16;
    TableMgr(bool is_new, Transaction& tx);
    auto createTable(const std::string& tbl_name, Schema& schema, Transaction& tx) -> void;
    auto getLayout(const std::string& tbl_name, Transaction& tx) -> Layout;

private:
    Layout tcat_layout;
    Layout fcat_layout;
};

#endif  // CDB_TABLEMGR_H
