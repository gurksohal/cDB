//
// Created by G on 10/30/2024.
//

#ifndef CDB_LOGRECORD_H
#define CDB_LOGRECORD_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "../../file/Page.h"
#include "../../log/LogManager.h"
#include "../Transaction.h"

class LogRecord {
public:
    enum Type : std::uint8_t { CHECKPOINT = 0, START = 1, COMMIT = 2, ROLLBACK = 3, SET_INT = 4, SET_STRING = 5 };

    LogRecord() = default;
    virtual ~LogRecord() = default;
    LogRecord(const LogRecord&&) = delete;
    LogRecord(const LogRecord&) = delete;
    auto operator=(const LogRecord&) = delete;
    auto operator=(LogRecord&&) = delete;
    virtual auto op() -> int = 0;
    virtual auto txNumber() -> int = 0;
    virtual auto undo(Transaction& tx) -> void = 0;
    virtual auto toString() -> std::string = 0;
    static auto createLogRecord(std::vector<std::byte>& bytes) -> std::unique_ptr<LogRecord>;
};

class CheckpointRecord : public LogRecord {
public:
    auto op() -> int override;
    auto txNumber() -> int override;
    auto undo(Transaction& tx) -> void override;
    auto toString() -> std::string override;
    static auto writeToLog(LogManager& lm) -> int;
};

class StartRecord : public LogRecord {
public:
    explicit StartRecord(Page& p);
    auto op() -> int override;
    auto txNumber() -> int override;
    auto undo(Transaction& tx) -> void override;
    auto toString() -> std::string override;
    static auto writeToLog(LogManager& lm, int tx) -> int;

private:
    int tx_num;
};

class CommitRecord : public LogRecord {
public:
    explicit CommitRecord(Page& p);
    auto op() -> int override;
    auto txNumber() -> int override;
    auto undo(Transaction& tx) -> void override;
    auto toString() -> std::string override;
    static auto writeToLog(LogManager& lm, int tx) -> int;

private:
    int tx_num;
};

class RollbackRecord : public LogRecord {
public:
    explicit RollbackRecord(Page& p);
    auto op() -> int override;
    auto txNumber() -> int override;
    auto undo(Transaction& tx) -> void override;
    auto toString() -> std::string override;
    static auto writeToLog(LogManager& lm, int tx) -> int;

private:
    int tx_num;
};

class SetIntRecord : public LogRecord {
public:
    explicit SetIntRecord(Page& p);
    auto op() -> int override;
    auto txNumber() -> int override;
    auto undo(Transaction& tx) -> void override;
    auto toString() -> std::string override;
    static auto writeToLog(LogManager& lm, int tx, BlockId& blk, int offset, int old_val) -> int;

private:
    int tx_num;
    int offset;
    int val;
    BlockId blk;
};

class SetStringRecord : public LogRecord {
public:
    explicit SetStringRecord(Page& p);
    auto op() -> int override;
    auto txNumber() -> int override;
    auto undo(Transaction& tx) -> void override;
    auto toString() -> std::string override;
    static auto writeToLog(LogManager& lm, int tx, BlockId& blk, int offset, std::string& old_val) -> int;

private:
    int tx_num;
    int offset;
    std::string val;
    BlockId blk;
};

#endif  // CDB_LOGRECORD_H
