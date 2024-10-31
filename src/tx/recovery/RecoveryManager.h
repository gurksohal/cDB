//
// Created by G on 10/30/2024.
//

#ifndef CDB_RECOVERYMANAGER_H
#define CDB_RECOVERYMANAGER_H

#include "../../buffer/BufferManager.h"
#include "../../log/LogManager.h"
class Transaction;

class RecoveryManager {
public:
    RecoveryManager(Transaction* tx, int tx_num, LogManager* lm, BufferManager* bm);
    auto commit() -> void;
    auto rollback() -> void;
    auto recover() -> void;
    auto setInt(Buffer& buff, int offset) -> int;
    auto setString(Buffer& buff, int offset) -> int;

private:
    auto doRollBack() -> void;
    auto doRecover() -> void;
    LogManager* lm;
    BufferManager* bm;
    Transaction* tx;
    int tx_num;
};

#endif  // CDB_RECOVERYMANAGER_H
