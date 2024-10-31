//
// Created by G on 10/30/2024.
//

#include "ConcurrencyManager.h"
LockTable ConcurrencyManager::lock_table;

auto ConcurrencyManager::sLock(BlockId& blk) -> void {
    if (!locks.contains(blk)) {
        lock_table.sLock(blk);
        locks[blk] = "S";
    }
}

auto ConcurrencyManager::xLock(BlockId& blk) -> void {
    if (!hasXLock(blk)) {
        sLock(blk);
        lock_table.xLock(blk);
        locks[blk] = "X";
    }
}

auto ConcurrencyManager::release() -> void {
    for (auto& entry : locks) {
        auto blk = entry.first;
        lock_table.unlock(blk);
    }
    locks.clear();
}

auto ConcurrencyManager::hasXLock(BlockId& blk) -> bool {
    if (!locks.contains(blk)) {
        return false;
    }
    return locks[blk] == "X";
}
