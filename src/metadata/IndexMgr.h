//
// Created by G on 11/15/2024.
//

#ifndef CDB_INDEXMGR_H
#define CDB_INDEXMGR_H

#include "../record/Layout.h"
#include "../tx/Transaction.h"
#include "IndexInfo.h"
#include "StatMgr.h"
#include "TableMgr.h"

class IndexMgr {
public:
    IndexMgr(bool is_new, TableMgr* tbl_m, StatMgr* stat_mgr, Transaction& tx);
    auto createIndex(const std::string& idx_name, const std::string& tbl_name, const std::string& fld_name,
                     Transaction& tx) -> void;
    auto getIndexInfo(const std::string& tbl_name, Transaction& tx) -> std::unordered_map<std::string, IndexInfo>;

private:
    TableMgr* tbl_mgr;
    StatMgr* stat_mgr;
    Layout layout;
};

#endif  // CDB_INDEXMGR_H
