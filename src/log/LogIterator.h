//
// Created by G on 10/26/2024.
//

#ifndef CDB_LOGITERATOR_H
#define CDB_LOGITERATOR_H

#include "../file/FileManager.h"

class LogIterator {
public:
    LogIterator(FileManager* fm, BlockId& block_id);
    auto hasNext() -> bool;
    auto next() -> std::vector<std::byte>;

private:
    auto moveToBlock(BlockId& bid) -> void;
    FileManager* file_manager;
    BlockId block_id;
    Page page;
    int current_pos;
    int boundary;
};

#endif  // CDB_LOGITERATOR_H
