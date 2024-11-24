//
// Created by G on 11/24/2024.
//

#ifndef CDB_MULTIBUFFERPRODUCTSCAN_H
#define CDB_MULTIBUFFERPRODUCTSCAN_H

#include "../query/Scan.h"
#include "../record/Layout.h"
#include "../tx/Transaction.h"

class MultiBufferProductScan : public Scan {
public:
    MultiBufferProductScan(Transaction* tx, std::shared_ptr<Scan> lhs_scan, const std::string& tbl_name, Layout layout);
    auto beforeFirst() -> void override;
    auto next() -> bool override;
    auto getInt(const std::string& fld_name) -> int override;
    auto getString(const std::string& fld_name) -> std::string override;
    auto getVal(const std::string& fld_name) -> Constant override;
    auto hasField(const std::string& fld_name) -> bool override;
    auto close() -> void override;

private:
    auto useNextChunk() -> bool;
    Transaction* tx;
    std::string file_name;
    std::shared_ptr<Scan> lhs_scan;
    std::shared_ptr<Scan> rhs_scan;
    std::shared_ptr<Scan> prod_scan;
    Layout layout;
    int chunk_size;
    int next_blk_num;
    int file_size;
};

#endif  // CDB_MULTIBUFFERPRODUCTSCAN_H
