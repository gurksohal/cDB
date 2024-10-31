//
// Created by G on 10/30/2024.
//

#ifndef CDB_BUFFERLIST_H
#define CDB_BUFFERLIST_H

#include <map>

#include "../buffer/Buffer.h"
#include "../buffer/BufferManager.h"
#include "../file/BlockId.h"

class BufferList {
public:
    explicit BufferList(BufferManager* bm);
    auto getBuffer(BlockId& blk) -> Buffer*;
    auto pin(BlockId& blk) -> void;
    auto unpin(const BlockId& blk) -> void;
    auto unpinAll() -> void;

private:
    std::map<BlockId, Buffer*> buffers;
    std::vector<BlockId> pins;
    BufferManager* buffer_manager;
};

#endif  // CDB_BUFFERLIST_H
