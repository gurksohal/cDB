//
// Created by G on 10/30/2024.
//

#include "LogRecord.h"

auto LogRecord::createLogRecord(std::vector<std::byte>& bytes) -> std::unique_ptr<LogRecord> {
    Page p(bytes);
    switch (p.getInt(0)) {
        case CHECKPOINT:
            return std::make_unique<CheckpointRecord>();
        case START:
            return std::make_unique<StartRecord>(p);
        case COMMIT:
            return std::make_unique<CommitRecord>(p);
        case ROLLBACK:
            return std::make_unique<RollbackRecord>(p);
        case SET_INT:
            return std::make_unique<SetIntRecord>(p);
        case SET_STRING:
            return std::make_unique<SetStringRecord>(p);
        default:
            throw std::runtime_error("invalid log entry type");
    }
}

auto CheckpointRecord::op() -> int {
    return CHECKPOINT;
}

auto CheckpointRecord::txNumber() -> int {
    return -1;
}

auto CheckpointRecord::undo(Transaction& tx) -> void {}

auto CheckpointRecord::toString() -> std::string {
    return "<CHECKPOINT>";
}

auto CheckpointRecord::writeToLog(LogManager& lm) -> int {
    Page p(sizeof(int));
    p.setInt(0, CHECKPOINT);
    return lm.append(p.getData());
}

StartRecord::StartRecord(Page& p) : tx_num(p.getInt(sizeof(int))) {}

auto StartRecord::op() -> int {
    return START;
}

auto StartRecord::txNumber() -> int {
    return tx_num;
}

auto StartRecord::undo(Transaction& tx) -> void {}

auto StartRecord::toString() -> std::string {
    return "<START " + std::to_string(tx_num) + ">";
}

auto StartRecord::writeToLog(LogManager& lm, int tx) -> int {
    Page p(2 * sizeof(int));
    p.setInt(0, START);
    p.setInt(sizeof(int), tx);
    return lm.append(p.getData());
}

CommitRecord::CommitRecord(Page& p) : tx_num(p.getInt(sizeof(int))) {}

auto CommitRecord::op() -> int {
    return COMMIT;
}

auto CommitRecord::txNumber() -> int {
    return tx_num;
}

auto CommitRecord::undo(Transaction& tx) -> void {}

auto CommitRecord::toString() -> std::string {
    return "<COMMIT " + std::to_string(tx_num) + ">";
}

auto CommitRecord::writeToLog(LogManager& lm, int tx) -> int {
    Page p(2 * sizeof(int));
    p.setInt(0, COMMIT);
    p.setInt(sizeof(int), tx);
    return lm.append(p.getData());
}

RollbackRecord::RollbackRecord(Page& p) : tx_num(p.getInt(sizeof(int))) {}

auto RollbackRecord::op() -> int {
    return ROLLBACK;
}

auto RollbackRecord::txNumber() -> int {
    return tx_num;
}

auto RollbackRecord::undo(Transaction& tx) -> void {}

auto RollbackRecord::toString() -> std::string {
    return "<ROLLBACK " + std::to_string(tx_num) + ">";
    ;
}

auto RollbackRecord::writeToLog(LogManager& lm, int tx) -> int {
    Page p(2 * sizeof(int));
    p.setInt(0, ROLLBACK);
    p.setInt(sizeof(int), tx);
    return lm.append(p.getData());
}

SetIntRecord::SetIntRecord(Page& p) : tx_num(p.getInt(sizeof(int))), blk("", -1) {
    int const fpos = static_cast<int>(sizeof(int)) + static_cast<int>(sizeof(int));
    std::string const filename = p.getString(fpos);
    int const b_pos = fpos + Page::maxLength(static_cast<int>(filename.length()));
    int const blk_num = p.getInt(b_pos);
    blk = BlockId(filename, blk_num);
    int const opos = b_pos + static_cast<int>(sizeof(int));
    offset = p.getInt(opos);
    int const v_pos = opos + static_cast<int>(sizeof(int));
    val = p.getInt(v_pos);
}

auto SetIntRecord::op() -> int {
    return SET_INT;
}

auto SetIntRecord::txNumber() -> int {
    return tx_num;
}

auto SetIntRecord::undo(Transaction& tx) -> void {
    tx.pin(blk);
    tx.setInt(blk, offset, val, false);
    tx.unpin(blk);
}

auto SetIntRecord::toString() -> std::string {
    return "<SETINT " + std::to_string(tx_num) + " " + blk.toString() + " " + std::to_string(offset) + " " +
           std::to_string(val) + ">";
}

auto SetIntRecord::writeToLog(LogManager& lm, int tx, BlockId& blk, int offset, int old_val) -> int {
    int const int_bytes = static_cast<int>(sizeof(int));
    int const t_pos = int_bytes;
    int const f_pos = t_pos + int_bytes;
    int const b_pos = f_pos + Page::maxLength(static_cast<int>(blk.filename().length()));
    int const o_pos = b_pos + int_bytes;
    int const v_pos = o_pos + int_bytes;
    Page p(v_pos + int_bytes);
    p.setInt(0, SET_INT);
    p.setInt(t_pos, tx);
    p.setString(f_pos, std::string(blk.filename()));
    p.setInt(b_pos, blk.blockNumber());
    int const val_o = offset;
    p.setInt(o_pos, val_o);
    p.setInt(v_pos, old_val);
    return lm.append(p.getData());
}

SetStringRecord::SetStringRecord(Page& p) : tx_num(p.getInt(sizeof(int))), blk("", -1) {
    int const int_bytes = static_cast<int>(sizeof(int));
    int const f_pos = int_bytes + int_bytes;
    std::string const filename = p.getString(f_pos);
    int const b_pos = f_pos + Page::maxLength(static_cast<int>(filename.length()));
    int const blk_num = p.getInt(b_pos);
    blk = BlockId(filename, blk_num);
    int const o_pos = b_pos + int_bytes;
    offset = p.getInt(o_pos);
    int const v_pos = o_pos + int_bytes;
    val = p.getString(v_pos);
}

auto SetStringRecord::op() -> int {
    return SET_STRING;
}

auto SetStringRecord::txNumber() -> int {
    return tx_num;
}

auto SetStringRecord::undo(Transaction& tx) -> void {
    tx.pin(blk);
    tx.setString(blk, offset, val, false);
    tx.unpin(blk);
}

auto SetStringRecord::toString() -> std::string {
    return "<SETSTRING " + std::to_string(tx_num) + " " + blk.toString() + " " + std::to_string(offset) + " " + val +
           ">";
}

auto SetStringRecord::writeToLog(LogManager& lm, int tx, BlockId& blk, int offset, std::string& old_val) -> int {
    int const int_bytes = static_cast<int>(sizeof(int));
    int const t_pos = int_bytes;
    int const f_pos = t_pos + int_bytes;
    int const b_pos = f_pos + Page::maxLength(static_cast<int>(blk.filename().length()));
    int const o_pos = b_pos + int_bytes;
    int const v_pos = o_pos + int_bytes;
    Page p(v_pos + Page::maxLength(static_cast<int>(old_val.length())));
    p.setInt(0, SET_STRING);
    p.setInt(t_pos, tx);
    p.setString(f_pos, std::string(blk.filename()));
    p.setInt(b_pos, blk.blockNumber());
    int const val_o = offset;
    p.setInt(o_pos, val_o);
    p.setString(v_pos, old_val);
    return lm.append(p.getData());
}
