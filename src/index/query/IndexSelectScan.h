//
// Created by G on 11/23/2024.
//

#ifndef CDB_INDEXSELECTSCAN_H
#define CDB_INDEXSELECTSCAN_H

#include <memory>

#include "../../query/Scan.h"
#include "../../record/TableScan.h"
#include "../Index.h"

class IndexSelectScan : public Scan {
public:
    IndexSelectScan(std::shared_ptr<TableScan> ts, std::shared_ptr<Index> idx, std::shared_ptr<Constant> val);
    auto beforeFirst() -> void override;
    auto next() -> bool override;
    auto getInt(const std::string& fld_name) -> int override;
    auto getString(const std::string& fld_name) -> std::string override;
    auto getVal(const std::string& fld_name) -> Constant override;
    auto hasField(const std::string& fld_name) -> bool override;
    auto close() -> void override;

private:
    std::shared_ptr<TableScan> ts;
    std::shared_ptr<Index> idx;
    std::shared_ptr<Constant> val;
};

#endif  // CDB_INDEXSELECTSCAN_H
