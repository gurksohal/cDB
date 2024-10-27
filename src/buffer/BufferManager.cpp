//
// Created by G on 10/26/2024.
//

#include "BufferManager.h"

#include <iostream>

BufferManager::BufferManager(FileManager *fm, LogManager *lm, int buffers)
    : buffer_pool(buffers, {fm, lm}), num_available(buffers) {}

auto BufferManager::available() -> int {
    std::unique_lock<std::mutex> const lock(mutex);
    return num_available;
}

auto BufferManager::flushAll(int tx_num) -> void {
    std::unique_lock<std::mutex> const lock(mutex);
    for (auto &buffer : buffer_pool) {
        if (buffer.modifyingTx() == tx_num) {
            buffer.flush();
        }
    }
}

auto BufferManager::unpin(Buffer &buff) -> void {
    std::unique_lock<std::mutex> lock(mutex);
    buff.unpin();
    if (!buff.isPinned()) {
        num_available++;
        lock.unlock();
        cond.notify_all();
    }
}

auto BufferManager::pin(BlockId &blk) -> Buffer & {
    std::unique_lock<std::mutex> lock(mutex);
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::milliseconds(max_time);
    auto *buff = tryToPin(blk);

    while (buff == nullptr && !waitingTooLong(start)) {
        cond.wait_until(lock, end);
        buff = tryToPin(blk);
    }
    if (buff == nullptr) {
        throw std::runtime_error("unable pin");
    }
    return *buff;
}

auto BufferManager::waitingTooLong(std::chrono::steady_clock::time_point start) -> bool {
    auto end = start + std::chrono::milliseconds(max_time);
    auto curr = std::chrono::steady_clock::now();
    return curr >= end;
}

auto BufferManager::tryToPin(BlockId &blk) -> Buffer * {
    auto *buff = findExistingBuffer(blk);

    if (buff == nullptr) {
        buff = chooseUnpinnedBuffer();
        if (buff == nullptr) {
            return nullptr;
        }

        buff->assignToBlock(blk);
    }
    if (!buff->isPinned()) {
        num_available--;
    }
    buff->pin();
    return buff;
}

auto BufferManager::findExistingBuffer(BlockId &blk) -> Buffer * {
    for (auto &buff : buffer_pool) {
        auto b = buff.block();
        if (b == blk) {
            return &buff;
        }
    }
    return nullptr;
}

auto BufferManager::chooseUnpinnedBuffer() -> Buffer * {
    for (auto &buff : buffer_pool) {
        if (!buff.isPinned()) {
            return &buff;
        }
    }
    return nullptr;
}
