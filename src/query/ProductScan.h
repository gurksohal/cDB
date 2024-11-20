//
// Created by G on 11/19/2024.
//

#ifndef CDB_PRODUCTSCAN_H
#define CDB_PRODUCTSCAN_H

#include <memory>

#include "Scan.h"

class ProductScan : public Scan {
public:
    ProductScan(std::shared_ptr<Scan>& s1, std::shared_ptr<Scan>& s2);
    auto beforeFirst() -> void override;
    auto next() -> bool override;
    auto getInt(const std::string& fld_name) -> int override;
    auto getString(const std::string& fld_name) -> std::string override;
    auto getVal(const std::string& fld_name) -> Constant override;
    auto hasField(const std::string& fld_name) -> bool override;
    auto close() -> void override;

private:
    std::shared_ptr<Scan> s1;
    std::shared_ptr<Scan> s2;
};

#endif  // CDB_PRODUCTSCAN_H
