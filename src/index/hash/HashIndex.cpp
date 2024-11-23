//
// Created by G on 11/23/2024.
//

#include "HashIndex.h"

#include <utility>

int const HashIndex::NUM_BUCKETS = 100;

HashIndex::HashIndex(Transaction *tx, std::string idx_name, Layout layout)
    : tx(tx), idx_name(std::move(idx_name)), layout(std::move(layout)) {}

auto HashIndex::next() -> bool {
    while (ts->next()) {
        if (ts->getVal("dataval") == *search_key) {
            return true;
        }
    }

    return false;
}

auto HashIndex::getDataRid() -> RID {
    int const blk_num = ts->getInt("block");
    int const id = ts->getInt("id");
    return {blk_num, id};
}

void HashIndex::insert(Constant &data_val, RID &data_rid) {
    beforeFirst(std::make_shared<Constant>(data_val));
    ts->insert();
    ts->setInt("block", data_rid.blockNumber());
    ts->setInt("id", data_rid.slot());
    ts->setVal("dataval", data_val);
}

void HashIndex::remove(Constant &data_val, RID &data_rid) {
    beforeFirst(std::make_shared<Constant>(data_val));
    while (next()) {
        if (getDataRid() == data_rid) {
            ts->remove();
            return;
        }
    }
}

void HashIndex::close() {
    if (ts) {
        ts->close();
    }
}

auto HashIndex::searchCost(int num_blocks) -> int {
    return num_blocks / HashIndex::NUM_BUCKETS;
}

auto HashIndex::beforeFirst(std::shared_ptr<Constant> key) -> void {
    close();
    std::ostringstream oss;
    oss << search_key;
    std::string const str = oss.str();
    this->search_key = key;
    int const bucket = static_cast<int>(std::hash<std::string> {}(str) % NUM_BUCKETS);
    std::string const tbl_name = idx_name + std::to_string(bucket);
    ts = std::make_shared<TableScan>(tx, tbl_name, layout);
}
