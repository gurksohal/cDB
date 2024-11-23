//
// Created by G on 11/24/2024.
//

#ifndef CDB_MERGEJOINSCAN_H
#define CDB_MERGEJOINSCAN_H

#include <memory>

#include "../query/Scan.h"
#include "SortScan.h"

class MergeJoinScan : public Scan {
public:
    MergeJoinScan(std::shared_ptr<Scan>& s1, std::shared_ptr<SortScan>& s2, std::string& fld_name1,
                  std::string& fld_name2);
    auto beforeFirst() -> void override;
    auto next() -> bool override;
    auto getInt(const std::string& fld_name) -> int override;
    auto getString(const std::string& fld_name) -> std::string override;
    auto getVal(const std::string& fld_name) -> Constant override;
    auto hasField(const std::string& fld_name) -> bool override;
    auto close() -> void override;

private:
    std::shared_ptr<Scan> s1;
    std::shared_ptr<SortScan> s2;
    std::string fld_name1;
    std::string fld_name2;
    // std::shared_ptr<Constant> join_val;
    Constant join_val;
};

#endif  // CDB_MERGEJOINSCAN_H
