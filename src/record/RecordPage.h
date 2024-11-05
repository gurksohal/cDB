//
// Created by G on 11/4/2024.
//

#ifndef CDB_RECORDPAGE_H
#define CDB_RECORDPAGE_H

#include <cstdint>

#include "../tx/Transaction.h"
#include "Layout.h"

class RecordPage {
public:
    enum State : std::uint8_t { EMPTY = 0, USED = 1 };

    RecordPage(Transaction* tx, BlockId blk, Layout* layout);
    auto getInt(int slot, const std::string& fld_name) -> int;
    auto getString(int slot, const std::string& fld_name) -> std::string;
    auto setInt(int slot, const std::string& fld_name, int val) -> void;
    auto setString(int slot, const std::string& fld_name, const std::string& val) -> void;
    auto remove(int slot) -> void;
    auto format() -> void;
    auto nextAfter(int slot) -> int;
    auto insertAfter(int slot) -> int;
    auto block() -> BlockId&;

private:
    auto setFlag(int slot, int flag) -> void;
    auto searchAfter(int slot, int flag) -> int;
    auto isValidSlot(int slot) -> bool;
    auto offset(int slot) -> int;
    Transaction* tx;
    BlockId blk;
    Layout* layout;
};

#endif  // CDB_RECORDPAGE_H
