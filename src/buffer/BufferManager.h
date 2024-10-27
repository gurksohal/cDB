//
// Created by G on 10/26/2024.
//

#ifndef CDB_BUFFERMANAGER_H
#define CDB_BUFFERMANAGER_H

#include <condition_variable>
#include <cstddef>

#include "Buffer.h"

class BufferManager {
public:
    BufferManager(FileManager* fm, LogManager* lm, int buffers);
    auto available() -> int;
    auto flushAll(int tx_num) -> void;
    auto unpin(Buffer& buff) -> void;
    auto pin(BlockId& blk) -> Buffer&;

private:
    auto waitingTooLong(std::chrono::steady_clock::time_point start) -> bool;
    auto tryToPin(BlockId& blk) -> Buffer*;
    auto findExistingBuffer(BlockId& blk) -> Buffer*;
    auto chooseUnpinnedBuffer() -> Buffer*;
    std::vector<Buffer> buffer_pool;
    int num_available;
    std::mutex mutex;
    std::condition_variable cond;
    const int64_t max_time = static_cast<int64_t>(10 * 1000);  // 10 sec
};

#endif  // CDB_BUFFERMANAGER_H
