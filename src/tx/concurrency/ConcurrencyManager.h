//
// Created by G on 10/30/2024.
//

#ifndef CDB_CONCURRENCYMANAGER_H
#define CDB_CONCURRENCYMANAGER_H

#include "LockTable.h"

class ConcurrencyManager {
public:
    auto sLock(BlockId& blk) -> void;
    auto xLock(BlockId& blk) -> void;
    auto release() -> void;

private:
    auto hasXLock(BlockId& blk) -> bool;
    static LockTable lock_table;
    std::map<BlockId, std::string> locks;
};

#endif  // CDB_CONCURRENCYMANAGER_H
