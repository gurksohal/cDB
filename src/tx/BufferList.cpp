//
// Created by G on 10/30/2024.
//

#include "BufferList.h"

BufferList::BufferList(BufferManager* bm) : buffer_manager(bm) {}

auto BufferList::getBuffer(BlockId& blk) -> Buffer* {
    return buffers[blk];
}

auto BufferList::pin(BlockId& blk) -> void {
    auto& buff = buffer_manager->pin(blk);
    buffers[blk] = &buff;
    pins.push_back(blk);
}

auto BufferList::unpin(const BlockId& blk) -> void {
    auto* buff = buffers[blk];
    buffer_manager->unpin(*buff);
    auto it = std::find(pins.begin(), pins.end(), blk);
    pins.erase(it);
    if (std::find(pins.begin(), pins.end(), blk) == pins.end()) {
        buffers.erase(blk);
    }
}

auto BufferList::unpinAll() -> void {
    for (auto& blk : pins) {
        auto* buff = buffers[blk];
        buffer_manager->unpin(*buff);
    }
    buffers.clear();
    pins.clear();
}
