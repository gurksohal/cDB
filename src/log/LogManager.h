//
// Created by G on 10/26/2024.
//

#ifndef CDB_LOGMANAGER_H
#define CDB_LOGMANAGER_H

#include "../file/FileManager.h"
#include "LogIterator.h"

class LogManager {
public:
    LogManager(FileManager* fm, const std::string& path);
    auto append(std::vector<std::byte>& rec) -> int;
    auto flush(int lsn) -> void;
    auto iterator() -> LogIterator;

private:
    auto appendNewBlock() -> BlockId;
    auto flush() -> void;
    FileManager* file_manager;
    std::string log_path;
    Page page;
    BlockId block_id;
    int latest_lsm;
    int last_saved_lsm;
    std::mutex mutex;
};

#endif  // CDB_LOGMANAGER_H
