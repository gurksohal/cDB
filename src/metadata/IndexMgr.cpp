//
// Created by G on 11/15/2024.
//

#include "IndexMgr.h"

#include "../record/TableScan.h"

IndexMgr::IndexMgr(bool is_new, TableMgr *tbl_mgr, StatMgr *stat_mgr, Transaction &tx)
    : tbl_mgr(tbl_mgr), stat_mgr(stat_mgr), layout(this->tbl_mgr->getLayout("idxcat", tx)) {
    if (is_new) {
        Schema sch;
        sch.addStringField("indexname", TableMgr::MAX_NAME);
        sch.addStringField("tablename", TableMgr::MAX_NAME);
        sch.addStringField("fieldname", TableMgr::MAX_NAME);
        tbl_mgr->createTable("idxcat", sch, tx);
    }

    this->tbl_mgr = tbl_mgr;
    this->stat_mgr = stat_mgr;
    this->layout = tbl_mgr->getLayout("idxcat", tx);
}

auto IndexMgr::createIndex(const std::string &idx_name, const std::string &tbl_name, const std::string &fld_name,
                           Transaction &tx) -> void {
    TableScan ts(&tx, "idxcat", layout);
    ts.insert();
    ts.setString("indexname", idx_name);
    ts.setString("tablename", tbl_name);
    ts.setString("fieldname", fld_name);
    ts.close();
}

auto IndexMgr::getIndexInfo(const std::string &tbl_name,
                            Transaction &tx) -> std::unordered_map<std::string, IndexInfo> {
    std::unordered_map<std::string, IndexInfo> result;
    TableScan ts(&tx, "idxcat", layout);
    while (ts.next()) {
        if (ts.getString("tablename") == tbl_name) {
            auto idx_name = ts.getString("indexname");
            auto fld_name = ts.getString("fieldname");
            auto tbl_layout = tbl_mgr->getLayout(tbl_name, tx);
            auto tbl_si = stat_mgr->getStatInfo(tbl_name, tbl_layout, tx);
            IndexInfo const ii(idx_name, fld_name, tbl_layout.schema(), &tx, tbl_si);
            result.insert({fld_name, ii});
        }
    }

    ts.close();
    return result;
}
