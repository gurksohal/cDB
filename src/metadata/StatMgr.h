//
// Created by G on 11/15/2024.
//

#ifndef CDB_STATMGR_H
#define CDB_STATMGR_H

#include "StatInfo.h"
#include "TableMgr.h"

class StatMgr {
public:
    StatMgr(TableMgr* tbl_mgr, Transaction& tx);
    auto getStatInfo(const std::string& tbl_name, Layout& layout, Transaction& tx) -> StatInfo;
    auto refreshStatistics(Transaction& tx) -> void;
    auto calcTableStats(const std::string& tbl_name, Layout& layout, Transaction& tx) -> StatInfo;

private:
    TableMgr* tbl_mgr;
    std::unordered_map<std::string, StatInfo> table_stats;
    int num_calls;
    std::recursive_mutex mutex;
};

#endif  // CDB_STATMGR_H
