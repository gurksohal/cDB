//
// Created by G on 11/23/2024.
//

#ifndef CDB_INDEXJOINSCAN_H
#define CDB_INDEXJOINSCAN_H

#include <memory>

#include "../../query/Scan.h"
#include "../../record/TableScan.h"
#include "../Index.h"

class IndexJoinScan : public Scan {
public:
    IndexJoinScan(std::shared_ptr<Scan> lhs, std::shared_ptr<Index> idx, std::string join_field,
                  std::shared_ptr<TableScan> rhs);
    auto beforeFirst() -> void override;
    auto next() -> bool override;
    auto getInt(const std::string& fld_name) -> int override;
    auto getString(const std::string& fld_name) -> std::string override;
    auto getVal(const std::string& fld_name) -> Constant override;
    auto hasField(const std::string& fld_name) -> bool override;
    auto close() -> void override;

private:
    auto resetIndex() -> void;
    std::shared_ptr<Scan> lhs;
    std::shared_ptr<Index> idx;
    std::string join_field;
    std::shared_ptr<TableScan> rhs;
};

#endif  // CDB_INDEXJOINSCAN_H
