//
// Created by G on 11/15/2024.
//

#include "StatMgr.h"

#include "../record/TableScan.h"

StatMgr::StatMgr(TableMgr *tbl_mgr, Transaction &tx) : tbl_mgr(tbl_mgr), num_calls(0) {
    refreshStatistics(tx);
}

auto StatMgr::getStatInfo(const std::string &tbl_name, Layout &layout, Transaction &tx) -> StatInfo {
    std::unique_lock<std::recursive_mutex> const lock(mutex);
    num_calls++;
    if (num_calls > 100) {
        refreshStatistics(tx);
    }

    if (table_stats.contains(tbl_name)) {
        return table_stats[tbl_name];
    }

    table_stats[tbl_name] = calcTableStats(tbl_name, layout, tx);
    return table_stats[tbl_name];
}

auto StatMgr::refreshStatistics(Transaction &tx) -> void {
    std::unique_lock<std::recursive_mutex> const lock(mutex);
    table_stats.clear();
    num_calls = 0;
    Layout const t_cat_layout = tbl_mgr->getLayout("tblcat", tx);
    TableScan t_cat(&tx, "tblcat", t_cat_layout);
    while (t_cat.next()) {
        auto tbl_name = t_cat.getString("tblname");
        auto layout = tbl_mgr->getLayout(tbl_name, tx);
        table_stats[tbl_name] = calcTableStats(tbl_name, layout, tx);
    }
    t_cat.close();
}

auto StatMgr::calcTableStats(const std::string &tbl_name, Layout &layout, Transaction &tx) -> StatInfo {
    std::unique_lock<std::recursive_mutex> const lock(mutex);
    int num_recs = 0;
    int num_blocks = 0;
    TableScan ts(&tx, tbl_name, layout);
    while (ts.next()) {
        num_recs++;
        num_blocks = ts.getRid().blockNumber() + 1;
    }
    ts.close();
    return {num_blocks, num_recs};
}
