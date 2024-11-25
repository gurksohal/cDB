//
// Created by G on 11/24/2024.
//

#ifndef CDB_CDB_H
#define CDB_CDB_H

#include <string>

#include "../buffer/BufferManager.h"
#include "../metadata/MetadataMgr.h"
#include "../plan/Planner.h"

class CDB {
public:
    static int const BLOCK_SIZE;
    static int const BUFFER_SIZE;
    static const char* LOG_FILE;
    CDB(std::filesystem::path& dir_name, int block_size, int buff_size);
    explicit CDB(std::filesystem::path& dir_name);
    auto nextTx() -> Transaction;
    auto mdMgr() -> MetadataMgr&;
    auto planner() -> Planner&;
    auto fileMgr() -> FileManager&;
    auto logMgr() -> LogManager&;
    auto bufferMgr() -> BufferManager&;
    auto execQuery(std::string& sql) -> void;
    auto printRows(const std::shared_ptr<Plan>& p) -> void;
    auto execUpdate(std::string& sql) -> int;
    auto rollback() -> void;
    auto commit() -> void;

private:
    std::unique_ptr<Transaction> curr_tx;
    std::unique_ptr<FileManager> fm;
    std::unique_ptr<LogManager> lm;
    std::unique_ptr<BufferManager> bm;
    std::unique_ptr<MetadataMgr> mdm;
    std::unique_ptr<Planner> planner_m;
};

#endif  // CDB_CDB_H
