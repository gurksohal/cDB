//
// Created by G on 10/30/2024.
//

#include "LockTable.h"
int LockTable::MAX_TIME = 10;

auto LockTable::sLock(BlockId& blk) -> void {
    std::unique_lock<std::mutex> lock(mutex);
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::seconds(LockTable::MAX_TIME);
    while (hasXLock(blk) && !waitingTooLong(start)) {
        cond.wait_until(lock, end);
    }
    if (hasXLock(blk)) {
        throw std::runtime_error("lock abort on sLock");
    }

    int const val = getLockVal(blk);
    locks[blk] = val + 1;
}

auto LockTable::xLock(BlockId& blk) -> void {
    std::unique_lock<std::mutex> lock(mutex);
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::seconds(LockTable::MAX_TIME);
    while (hasOtherSLocks(blk) && !waitingTooLong(start)) {
        cond.wait_until(lock, end);
    }
    if (hasOtherSLocks(blk)) {
        throw std::runtime_error("lock abort on xLock");
    }
    locks[blk] = -1;
}

auto LockTable::unlock(BlockId& blk) -> void {
    std::unique_lock<std::mutex> lock(mutex);
    int const val = getLockVal(blk);
    if (val > 1) {
        locks[blk] = val - 1;
    } else {
        locks.erase(blk);
        lock.unlock();
        cond.notify_all();
    }
}

auto LockTable::hasXLock(BlockId& blk) -> bool {
    return getLockVal(blk) < 0;
}

auto LockTable::hasOtherSLocks(BlockId& blk) -> bool {
    return getLockVal(blk) > 1;
}

auto LockTable::waitingTooLong(std::chrono::steady_clock::time_point start) -> bool {
    auto end = start + std::chrono::milliseconds(LockTable::MAX_TIME);
    auto curr = std::chrono::steady_clock::now();
    return curr >= end;
}

auto LockTable::getLockVal(BlockId& blk) -> int {
    return locks[blk];
}
