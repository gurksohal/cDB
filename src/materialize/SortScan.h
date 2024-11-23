//
// Created by G on 11/24/2024.
//

#ifndef CDB_SORTSCAN_H
#define CDB_SORTSCAN_H

#include <memory>

#include "../query/Scan.h"
#include "../query/UpdateScan.h"
#include "RecordComparator.h"
#include "TempTable.h"

class SortScan : public Scan {
public:
    SortScan(std::vector<std::unique_ptr<TempTable>>& runs, RecordComparator& comp);
    auto beforeFirst() -> void override;
    auto next() -> bool override;
    auto getInt(const std::string& fld_name) -> int override;
    auto getString(const std::string& fld_name) -> std::string override;
    auto getVal(const std::string& fld_name) -> Constant override;
    auto hasField(const std::string& fld_name) -> bool override;
    auto close() -> void override;
    auto savePosition() -> void;
    auto restorePosition() -> void;

private:
    std::shared_ptr<TableScan> s1;
    std::shared_ptr<TableScan> s2;
    std::shared_ptr<TableScan> curr_scan;
    RecordComparator comp;
    bool has_more1;
    bool has_more2;
    std::vector<RID> saved_position;
};

#endif  // CDB_SORTSCAN_H
