//
// Created by G on 11/4/2024.
//

#ifndef CDB_TABLESCAN_H
#define CDB_TABLESCAN_H

#include "../query/UpdateScan.h"
#include "../tx/Transaction.h"
#include "Layout.h"
#include "RID.h"
#include "RecordPage.h"

class TableScan : public UpdateScan {
public:
    TableScan(Transaction* tx, const std::string& tbl_name, const Layout& layout);
    auto beforeFirst() -> void override;
    auto next() -> bool override;
    auto getInt(const std::string& fld_name) -> int override;
    auto getString(const std::string& fld_name) -> std::string override;
    auto getVal(const std::string& fld_name) -> Constant override;
    auto hasField(const std::string& fld_name) -> bool override;
    auto close() -> void override;
    auto setInt(const std::string& fld_name, int val) -> void override;
    auto setString(const std::string& fld_name, const std::string& val) -> void override;
    auto setVal(const std::string& fld_name, Constant val) -> void override;
    auto insert() -> void override;
    auto remove() -> void override;
    auto moveToRID(const RID& rid) -> void override;
    auto getRid() -> RID override;

private:
    auto moveToBlock(int blk_num) -> void;
    auto moveToNewBlock() -> void;
    auto atLastBlock() -> bool;
    Transaction* tx;
    Layout layout;
    RecordPage rp;
    std::string filename;
    int current_slot;
};

#endif  // CDB_TABLESCAN_H
