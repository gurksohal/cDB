//
// Created by G on 10/30/2024.
//

#ifndef CDB_LOCKTABLE_H
#define CDB_LOCKTABLE_H

#include <condition_variable>
#include <map>
#include <mutex>

#include "../../file/BlockId.h"

class LockTable {
public:
    auto sLock(BlockId& blk) -> void;
    auto xLock(BlockId& blk) -> void;
    auto unlock(BlockId& blk) -> void;

private:
    auto hasXLock(BlockId& blk) -> bool;
    auto hasOtherSLocks(BlockId& blk) -> bool;
    static auto waitingTooLong(std::chrono::steady_clock::time_point start) -> bool;
    auto getLockVal(BlockId& blk) -> int;
    static int MAX_TIME;
    std::map<BlockId, int> locks;
    std::mutex mutex;
    std::condition_variable cond;
};

#endif  // CDB_LOCKTABLE_H
