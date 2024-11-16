//
// Created by G on 11/15/2024.
//

#include "MetadataMgr.h"

MetadataMgr::MetadataMgr(bool is_new, Transaction& tx)
    : tbl_mgr(is_new, tx),
      view_mgr(is_new, &tbl_mgr, tx),
      stat_mgr(&tbl_mgr, tx),
      idx_mgr(is_new, &tbl_mgr, &stat_mgr, tx) {}

auto MetadataMgr::createTable(const std::string& tbl_name, Schema& sch, Transaction& tx) -> void {
    tbl_mgr.createTable(tbl_name, sch, tx);
}

auto MetadataMgr::getLayout(const std::string& tbl_name, Transaction& tx) -> Layout {
    return tbl_mgr.getLayout(tbl_name, tx);
}

auto MetadataMgr::createView(const std::string& view_name, const std::string& view_def, Transaction& tx) -> void {
    view_mgr.createView(view_name, view_def, tx);
}

auto MetadataMgr::getViewDef(const std::string& view_name, Transaction& tx) -> std::string {
    return view_mgr.getViewDef(view_name, tx);
}

auto MetadataMgr::createIndex(const std::string& idx_name, const std::string& tbl_name, const std::string& fld_name,
                              Transaction& tx) -> void {
    idx_mgr.createIndex(idx_name, tbl_name, fld_name, tx);
}

auto MetadataMgr::getIndexInfo(const std::string& tbl_name,
                               Transaction& tx) -> std::unordered_map<std::string, IndexInfo> {
    return std::move(idx_mgr.getIndexInfo(tbl_name, tx));
}

auto MetadataMgr::getStatInfo(const std::string& tbl_name, Layout& layout, Transaction& tx) -> StatInfo {
    return stat_mgr.getStatInfo(tbl_name, layout, tx);
}
