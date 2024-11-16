//
// Created by G on 11/15/2024.
//

#ifndef CDB_METADATAMGR_H
#define CDB_METADATAMGR_H

#include "IndexMgr.h"
#include "TableMgr.h"
#include "ViewMgr.h"

class MetadataMgr {
public:
    MetadataMgr(bool is_new, Transaction& tx);
    auto createTable(const std::string& tbl_name, Schema& sch, Transaction& tx) -> void;
    auto getLayout(const std::string& tbl_name, Transaction& tx) -> Layout;
    auto createView(const std::string& view_name, const std::string& view_def, Transaction& tx) -> void;
    auto getViewDef(const std::string& view_name, Transaction& tx) -> std::string;
    auto createIndex(const std::string& idx_name, const std::string& tbl_name, const std::string& fld_name,
                     Transaction& tx) -> void;
    auto getIndexInfo(const std::string& tbl_name, Transaction& tx) -> std::unordered_map<std::string, IndexInfo>;
    auto getStatInfo(const std::string& tbl_name, Layout& layout, Transaction& tx) -> StatInfo;

private:
    TableMgr tbl_mgr;
    ViewMgr view_mgr;
    StatMgr stat_mgr;
    IndexMgr idx_mgr;
};

#endif  // CDB_METADATAMGR_H
