//
// Created by G on 11/23/2024.
//

#ifndef CDB_INDEX_H
#define CDB_INDEX_H
#include "../query/Constant.h"
#include "../record/RID.h"
#include "memory"

class Index {
public:
    Index() = default;
    Index(Index&) = delete;
    Index(Index&&) = delete;
    auto operator=(Index&) = delete;
    auto operator=(Index&&) = delete;
    virtual ~Index() = default;
    virtual auto beforeFirst(std::shared_ptr<Constant> key) -> void = 0;
    virtual auto next() -> bool = 0;
    virtual auto getDataRid() -> RID = 0;
    virtual auto insert(Constant& data_val, RID& data_rid) -> void = 0;
    virtual auto remove(Constant& data_val, RID& data_rid) -> void = 0;
    virtual auto close() -> void = 0;
};
#endif  // CDB_INDEX_H
