//
// Created by G on 10/30/2024.
//

#include "RecoveryManager.h"

#include "LogRecord.h"

RecoveryManager::RecoveryManager(Transaction *tx, int tx_num, LogManager *lm, BufferManager *bm)
    : lm(lm), bm(bm), tx(tx), tx_num(tx_num) {
    StartRecord::writeToLog(*lm, tx_num);
}

auto RecoveryManager::commit() -> void {
    bm->flushAll(tx_num);
    int const lsn = CommitRecord::writeToLog(*lm, tx_num);
    lm->flush(lsn);
}

auto RecoveryManager::rollback() -> void {
    doRollBack();
    bm->flushAll(tx_num);
    int const lsn = RollbackRecord::writeToLog(*lm, tx_num);
    lm->flush(lsn);
}

auto RecoveryManager::recover() -> void {
    doRecover();
    bm->flushAll(tx_num);
    int const lsn = CheckpointRecord::writeToLog(*lm);
    lm->flush(lsn);
}

auto RecoveryManager::setInt(Buffer &buff, int offset) -> int {
    int const old_val = buff.contents()->getInt(offset);
    BlockId blk = buff.block();
    return SetIntRecord::writeToLog(*lm, tx_num, blk, offset, old_val);
}

auto RecoveryManager::setString(Buffer &buff, int offset) -> int {
    std::string old_val = buff.contents()->getString(offset);
    BlockId blk = buff.block();
    return SetStringRecord::writeToLog(*lm, tx_num, blk, offset, old_val);
}

auto RecoveryManager::doRollBack() -> void {
    auto it = lm->iterator();
    while (it.hasNext()) {
        auto bytes = it.next();
        auto rec = LogRecord::createLogRecord(bytes);
        if (rec->txNumber() == tx_num) {
            if (rec->op() == LogRecord::START) {
                return;
            }
            rec->undo(*tx);
        }
    }
}

auto RecoveryManager::doRecover() -> void {
    std::vector<int> finished_txs;
    auto it = lm->iterator();
    while (it.hasNext()) {
        auto bytes = it.next();
        auto rec = LogRecord::createLogRecord(bytes);
        if (rec->op() == LogRecord::CHECKPOINT) {
            return;
        }
        if (rec->op() == LogRecord::COMMIT || rec->op() == LogRecord::ROLLBACK) {
            finished_txs.push_back(rec->txNumber());
        } else if (std::find(finished_txs.begin(), finished_txs.end(), rec->txNumber()) == finished_txs.end()) {
            rec->undo(*tx);
        }
    }
}
