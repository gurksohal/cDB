//
// Created by G on 11/23/2024.
//

#ifndef CDB_HASHINDEX_H
#define CDB_HASHINDEX_H

#include "../../record/Layout.h"
#include "../../record/TableScan.h"
#include "../../tx/Transaction.h"
#include "../Index.h"

class HashIndex : public Index {
public:
    static int const NUM_BUCKETS;
    HashIndex(Transaction* tx, std::string idx_name, Layout layout);
    auto beforeFirst(std::shared_ptr<Constant> key) -> void override;
    auto next() -> bool override;
    auto getDataRid() -> RID override;
    auto insert(Constant& data_val, RID& data_rid) -> void override;
    auto remove(Constant& data_val, RID& data_rid) -> void override;
    auto close() -> void override;
    auto static searchCost(int num_blocks) -> int;

private:
    Transaction* tx;
    std::string idx_name;
    Layout layout;
    std::shared_ptr<Constant> search_key;
    std::shared_ptr<TableScan> ts;
};

#endif  // CDB_HASHINDEX_H
