//
// Created by G on 10/30/2024.
//

#ifndef CDB_TRANSACTION_H
#define CDB_TRANSACTION_H

#include "BufferList.h"
#include "concurrency/ConcurrencyManager.h"
#include "recovery/RecoveryManager.h"

class Transaction {
public:
    Transaction(FileManager* fm, LogManager* lm, BufferManager* bm);
    auto commit() -> void;
    auto rollback() -> void;
    auto recover() -> void;
    auto pin(BlockId& blk) -> void;
    auto unpin(BlockId& blk) -> void;
    auto getInt(BlockId& blk, int offset) -> int;
    auto getString(BlockId& blk, int offset) -> std::string;
    auto setInt(BlockId& blk, int offset, int val, bool ok_to_log) -> void;
    auto setString(BlockId& blk, int offset, const std::string& val, bool ok_to_log) -> void;
    auto size(std::string& filename) -> int;
    auto append(std::string& filename) -> BlockId;
    auto blockSize() -> int;
    auto availableBuffs() -> int;
    static auto nextTxNumber() -> int;

private:
    static int next_tx_num;
    static int const END_OF_FILE = -1;
    RecoveryManager recovery_manager;
    ConcurrencyManager concurrency_manager;
    BufferManager* bm;
    FileManager* fm;
    int tx_num;
    BufferList my_buffers;
    static std::mutex mutex;
};

#endif  // CDB_TRANSACTION_H
