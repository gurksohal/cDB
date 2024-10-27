//
// Created by G on 10/26/2024.
//

#ifndef CDB_BUFFER_H
#define CDB_BUFFER_H

#include "../log/LogManager.h"

class Buffer {
public:
    Buffer(FileManager* fm, LogManager* lm);
    auto contents() -> Page*;
    auto block() -> BlockId;
    auto setModified(int new_tx_num, int new_lsn) -> void;
    auto isPinned() const -> bool;
    auto modifyingTx() const -> int;
    auto assignToBlock(BlockId& b) -> void;
    auto flush() -> void;
    auto pin() -> void;
    auto unpin() -> void;

private:
    FileManager* file_manager;
    LogManager* log_manager;
    Page page;
    BlockId block_id;
    int pins;
    int tx_num;
    int lsn;
};

#endif  // CDB_BUFFER_H
