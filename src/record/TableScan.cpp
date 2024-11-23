//
// Created by G on 11/4/2024.
//

#include "TableScan.h"

TableScan::TableScan(Transaction *tx, const std::string &tbl_name, const Layout &layout)
    : tx(tx), layout(layout), rp(tx, {"", -1}, layout), current_slot(0) {
    filename = tbl_name + ".tbl";
    if (tx->size(filename) == 0) {
        moveToNewBlock();
    } else {
        moveToBlock(0);
    }
}

auto TableScan::beforeFirst() -> void {
    moveToBlock(0);
}

auto TableScan::next() -> bool {
    current_slot = rp.nextAfter(current_slot);
    while (current_slot < 0) {
        if (atLastBlock()) {
            return false;
        }
        moveToBlock(rp.block().blockNumber() + 1);
        current_slot = rp.nextAfter(current_slot);
    }
    return true;
}

auto TableScan::getInt(const std::string &fld_name) -> int {
    return rp.getInt(current_slot, fld_name);
}

auto TableScan::getString(const std::string &fld_name) -> std::string {
    return rp.getString(current_slot, fld_name);
}

auto TableScan::getVal(const std::string &fld_name) -> Constant {
    if (layout.schema().type(fld_name) == Schema::INTEGER) {
        return Constant(getInt(fld_name));
    }
    return Constant(getString(fld_name));
}

auto TableScan::hasField(const std::string &fld_name) -> bool {
    return layout.schema().hasField(fld_name);
}

auto TableScan::close() -> void {
    if (!rp.block().filename().empty()) {
        tx->unpin(rp.block());
    }
}

auto TableScan::setInt(const std::string &fld_name, int val) -> void {
    rp.setInt(current_slot, fld_name, val);
}

auto TableScan::setString(const std::string &fld_name, const std::string &val) -> void {
    rp.setString(current_slot, fld_name, val);
}

auto TableScan::setVal(const std::string &fld_name, Constant val) -> void {
    if (layout.schema().type(fld_name) == Schema::INTEGER) {
        setInt(fld_name, val.asInt());
    } else {
        setString(fld_name, val.asString());
    }
}

auto TableScan::insert() -> void {
    current_slot = rp.insertAfter(current_slot);
    while (current_slot < 0) {
        if (atLastBlock()) {
            moveToNewBlock();
        } else {
            moveToBlock(rp.block().blockNumber() + 1);
        }
        current_slot = rp.insertAfter(current_slot);
    }
}

auto TableScan::remove() -> void {
    rp.remove(current_slot);
}

auto TableScan::moveToRID(const RID &rid) -> void {
    close();
    BlockId const block_id {filename, rid.blockNumber()};
    rp = RecordPage(tx, block_id, layout);
    current_slot = rid.slot();
}

auto TableScan::getRid() -> RID {
    return {rp.block().blockNumber(), current_slot};
}

auto TableScan::moveToBlock(int blk_num) -> void {
    if (!rp.block().filename().empty()) {
        tx->unpin(rp.block());
    }
    BlockId const block_id {filename, blk_num};
    rp = RecordPage(tx, block_id, layout);
    current_slot = -1;
}

auto TableScan::moveToNewBlock() -> void {
    if (!rp.block().filename().empty()) {
        tx->unpin(rp.block());
    }
    BlockId const block_id = tx->append(filename);
    rp = RecordPage(tx, block_id, layout);
    rp.format();
    current_slot = -1;
}

auto TableScan::atLastBlock() -> bool {
    return rp.block().blockNumber() == tx->size(filename) - 1;
}
