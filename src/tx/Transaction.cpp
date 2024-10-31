//
// Created by G on 10/30/2024.
//

#include "Transaction.h"
int Transaction::next_tx_num = 0;
std::mutex Transaction::mutex;

Transaction::Transaction(FileManager *fm, LogManager *lm, BufferManager *bm)
    : fm(fm), bm(bm), tx_num(Transaction::nextTxNumber()), recovery_manager(this, -1, lm, bm), my_buffers(bm) {
    recovery_manager = RecoveryManager(this, tx_num, lm, bm);
}

auto Transaction::commit() -> void {
    recovery_manager.commit();
    concurrency_manager.release();
    my_buffers.unpinAll();
}

auto Transaction::rollback() -> void {
    recovery_manager.rollback();
    concurrency_manager.release();
    my_buffers.unpinAll();
}

auto Transaction::recover() -> void {
    bm->flushAll(tx_num);
    recovery_manager.recover();
}

auto Transaction::pin(BlockId &blk) -> void {
    my_buffers.pin(blk);
}

auto Transaction::unpin(BlockId &blk) -> void {
    my_buffers.unpin(blk);
}

auto Transaction::getInt(BlockId &blk, int offset) -> int {
    concurrency_manager.sLock(blk);
    Buffer *buff = my_buffers.getBuffer(blk);
    return buff->contents()->getInt(offset);
}

auto Transaction::getString(BlockId &blk, int offset) -> std::string {
    concurrency_manager.sLock(blk);
    Buffer *buff = my_buffers.getBuffer(blk);
    return buff->contents()->getString(offset);
}

auto Transaction::setInt(BlockId &blk, int offset, int val, bool ok_to_log) -> void {
    concurrency_manager.xLock(blk);
    Buffer *buff = my_buffers.getBuffer(blk);
    int lsn = -1;
    if (ok_to_log) {
        lsn = recovery_manager.setInt(*buff, offset);
    }

    Page *p = buff->contents();
    p->setInt(offset, val);
    buff->setModified(tx_num, lsn);
}

auto Transaction::setString(BlockId &blk, int offset, const std::string &val, bool ok_to_log) -> void {
    concurrency_manager.xLock(blk);
    Buffer *buff = my_buffers.getBuffer(blk);
    int lsn = -1;
    if (ok_to_log) {
        lsn = recovery_manager.setString(*buff, offset);
    }
    Page *p = buff->contents();
    p->setString(offset, val);
    buff->setModified(tx_num, lsn);
}

auto Transaction::size(std::string &filename) -> int {
    BlockId blk(filename, END_OF_FILE);
    concurrency_manager.sLock(blk);
    return fm->length(filename);
}

auto Transaction::append(std::string &filename) -> BlockId {
    BlockId blk(filename, END_OF_FILE);
    concurrency_manager.xLock(blk);
    return fm->append(filename);
}

auto Transaction::blockSize() -> int {
    return fm->blockSize();
}

auto Transaction::availableBuffs() -> int {
    return bm->available();
}

auto Transaction::nextTxNumber() -> int {
    std::unique_lock<std::mutex> const lock(Transaction::mutex);
    Transaction::next_tx_num++;
    return Transaction::next_tx_num;
}
