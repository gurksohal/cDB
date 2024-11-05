//
// Created by G on 11/4/2024.
//

#ifndef CDB_TABLESCAN_H
#define CDB_TABLESCAN_H

#include "../tx/Transaction.h"
#include "Layout.h"
#include "RID.h"
#include "RecordPage.h"

class TableScan {  // TODO: extend updateScan
public:
    TableScan(Transaction* tx, const std::string& tbl_name, Layout* layout);
    auto beforeFirst() -> void;
    auto next() -> bool;
    auto getInt(const std::string& fld_name) -> int;
    auto getString(const std::string& fld_name) -> std::string;
    auto getVal(const std::string& fld_name) -> void;  // TODO: Fix after Query to include Constant
    auto hasField(const std::string& fld_name) -> bool;
    auto close() -> void;
    auto setInt(const std::string& fld_name, int val) -> void;
    auto setString(const std::string& fld_name, const std::string& val) -> void;
    auto setVal(const std::string& fld_name, int val) -> void;  // TODO: Fix after Query to include Constant
    auto insert() -> void;
    auto remove() -> void;
    auto moveToRID(const RID& rid) -> void;
    auto getRid() -> RID;

private:
    auto moveToBlock(int blk_num) -> void;
    auto moveToNewBlock() -> void;
    auto atLastBlock() -> bool;
    Transaction* tx;
    Layout* layout;
    RecordPage rp;
    std::string filename;
    int current_slot;
};

#endif  // CDB_TABLESCAN_H
