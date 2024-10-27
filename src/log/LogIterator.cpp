//
// Created by G on 10/26/2024.
//

#include "LogIterator.h"

LogIterator::LogIterator(FileManager* fm, BlockId& block_id)
    : file_manager(fm), block_id(block_id), page(file_manager->blockSize()), current_pos(0), boundary(0) {
    moveToBlock(block_id);
}

auto LogIterator::hasNext() -> bool {
    return current_pos < file_manager->blockSize() || block_id.blockNumber() > 0;
}

auto LogIterator::next() -> std::vector<std::byte> {
    if (current_pos == file_manager->blockSize()) {
        block_id = BlockId(std::string(block_id.filename()), block_id.blockNumber() - 1);
        moveToBlock(block_id);
    }
    auto rec = page.getBytes(current_pos);
    current_pos += static_cast<int>(sizeof(int)) + static_cast<int>(rec.size());
    return rec;
}

auto LogIterator::moveToBlock(BlockId& bid) -> void {
    file_manager->read(bid, page);
    boundary = page.getInt(0);
    current_pos = boundary;
}
