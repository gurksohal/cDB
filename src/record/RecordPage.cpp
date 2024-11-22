//
// Created by G on 11/4/2024.
//

#include "RecordPage.h"

#include <utility>

RecordPage::RecordPage(Transaction *tx, BlockId blk, Layout layout)
    : tx(tx), blk(std::move(blk)), layout(std::move(layout)) {
    tx->pin(this->blk);
}

auto RecordPage::getInt(int slot, const std::string &fld_name) -> int {
    int const fld_pos = offset(slot) + layout.offset(fld_name);
    return tx->getInt(blk, fld_pos);
}

auto RecordPage::getString(int slot, const std::string &fld_name) -> std::string {
    int const fld_pos = offset(slot) + layout.offset(fld_name);
    return tx->getString(blk, fld_pos);
}

auto RecordPage::setInt(int slot, const std::string &fld_name, int val) -> void {
    int const fld_pos = offset(slot) + layout.offset(fld_name);
    tx->setInt(blk, fld_pos, val, true);
}

auto RecordPage::setString(int slot, const std::string &fld_name, const std::string &val) -> void {
    int const fld_pos = offset(slot) + layout.offset(fld_name);
    tx->setString(blk, fld_pos, val, true);
}

auto RecordPage::remove(int slot) -> void {
    setFlag(slot, EMPTY);
}

auto RecordPage::format() -> void {
    int slot = 0;
    while (isValidSlot(slot)) {
        tx->setInt(blk, offset(slot), EMPTY, false);
        auto sch = layout.schema();
        for (auto &name : sch.fields()) {
            int const fld_pos = offset(slot) + layout.offset(name);
            if (sch.type(name) == Schema::INTEGER) {
                tx->setInt(blk, fld_pos, 0, false);
            } else {
                tx->setString(blk, fld_pos, "", false);
            }
        }
        slot++;
    }
}

auto RecordPage::nextAfter(int slot) -> int {
    return searchAfter(slot, USED);
}

auto RecordPage::insertAfter(int slot) -> int {
    int const new_slot = searchAfter(slot, EMPTY);
    if (new_slot >= 0) {
        setFlag(new_slot, USED);
    }
    return new_slot;
}

auto RecordPage::block() -> BlockId & {
    return blk;
}

auto RecordPage::setFlag(int slot, int flag) -> void {
    tx->setInt(blk, offset(slot), flag, true);
}

auto RecordPage::searchAfter(int slot, int flag) -> int {
    slot++;
    while (isValidSlot(slot)) {
        if (tx->getInt(blk, offset(slot)) == flag) {
            return slot;
        }
        slot++;
    }
    return -1;
}

auto RecordPage::isValidSlot(int slot) -> bool {
    return offset(slot + 1) <= tx->blockSize();
}

auto RecordPage::offset(int slot) -> int {
    return slot * layout.slotSize();
}
