//
// Created by G on 11/24/2024.
//

#include "MultiBufferProductScan.h"

#include <utility>

#include "../query/ProductScan.h"
#include "BufferNeeds.h"
#include "ChunkScan.h"

MultiBufferProductScan::MultiBufferProductScan(Transaction *tx, std::shared_ptr<Scan> lhs_scan,
                                               const std::string &tbl_name, Layout layout)
    : tx(tx),
      lhs_scan(std::move(lhs_scan)),
      layout(std::move(layout)),
      file_name(tbl_name + ".tbl"),
      chunk_size(0),
      next_blk_num(0) {
    auto s = tx->size(file_name);
    file_size = s;

    int const available = tx->availableBuffs();
    chunk_size = BufferNeeds::bestFactor(available, file_size);
    useNextChunk();
}

auto MultiBufferProductScan::useNextChunk() -> bool {
    if (next_blk_num >= file_size) {
        return false;
    }
    if (rhs_scan) {
        rhs_scan->close();
    }
    int end = next_blk_num + chunk_size - 1;
    if (end >= file_size) {
        end = file_size - 1;
    }
    rhs_scan = std::make_shared<ChunkScan>(tx, file_name, layout, next_blk_num, end);
    lhs_scan->beforeFirst();
    prod_scan = std::make_shared<ProductScan>(lhs_scan, rhs_scan);
    next_blk_num = end + 1;
    return true;
}

void MultiBufferProductScan::beforeFirst() {
    next_blk_num = 0;
    useNextChunk();
}

auto MultiBufferProductScan::next() -> bool {
    while (!prod_scan->next()) {
        if (!useNextChunk()) {
            return false;
        }
    }
    return true;
}

auto MultiBufferProductScan::getInt(const std::string &fld_name) -> int {
    return prod_scan->getInt(fld_name);
}

auto MultiBufferProductScan::getString(const std::string &fld_name) -> std::string {
    return prod_scan->getString(fld_name);
}

auto MultiBufferProductScan::getVal(const std::string &fld_name) -> Constant {
    return prod_scan->getVal(fld_name);
}

auto MultiBufferProductScan::hasField(const std::string &fld_name) -> bool {
    return prod_scan->hasField(fld_name);
}

void MultiBufferProductScan::close() {
    prod_scan->close();
}
