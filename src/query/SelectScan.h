//
// Created by G on 11/19/2024.
//

#ifndef CDB_SELECTSCAN_H
#define CDB_SELECTSCAN_H

#include <memory>

#include "Predicate.h"
#include "UpdateScan.h"

class SelectScan : public UpdateScan {
public:
    auto beforeFirst() -> void override;
    auto next() -> bool override;
    auto getInt(const std::string& fld_name) -> int override;
    auto getString(const std::string& fld_name) -> std::string override;
    auto getVal(const std::string& fld_name) -> Constant override;
    auto hasField(const std::string& fld_name) -> bool override;
    auto close() -> void override;
    auto setVal(const std::string& fld_name, Constant val) -> void override;
    auto setInt(const std::string& fld_name, int val) -> void override;
    auto setString(const std::string& fld_name, const std::string& val) -> void override;
    auto insert() -> void override;
    auto remove() -> void override;
    auto getRid() -> RID override;
    auto moveToRID(const RID& rid) -> void override;
    SelectScan(std::shared_ptr<Scan> s, Predicate& p);

private:
    std::shared_ptr<Scan> scan;
    Predicate pred;
};

#endif  // CDB_SELECTSCAN_H
