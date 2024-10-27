//
// Created by G on 10/26/2024.
//

#include "Buffer.h"

Buffer::Buffer(FileManager *fm, LogManager *lm)
    : file_manager(fm),
      log_manager(lm),
      page(file_manager->blockSize()),
      block_id("", -1),
      pins(0),
      tx_num(-1),
      lsn(-1) {}

auto Buffer::contents() -> Page * {
    return &page;
}

auto Buffer::block() -> BlockId {
    return block_id;
}

auto Buffer::setModified(int new_tx_num, int new_lsn) -> void {
    tx_num = new_tx_num;
    if (new_lsn >= 0) {
        lsn = new_lsn;
    }
}

auto Buffer::isPinned() const -> bool {
    return pins > 0;
}

auto Buffer::modifyingTx() const -> int {
    return tx_num;
}

auto Buffer::assignToBlock(BlockId &b) -> void {
    flush();
    block_id = b;
    file_manager->read(block_id, page);
    pins = 0;
}

auto Buffer::flush() -> void {
    if (tx_num >= 0 && !block_id.filename().empty()) {
        log_manager->flush(lsn);
        file_manager->write(block_id, page);
        tx_num = -1;
    }
}

auto Buffer::pin() -> void {
    pins++;
}

auto Buffer::unpin() -> void {
    pins--;
}
