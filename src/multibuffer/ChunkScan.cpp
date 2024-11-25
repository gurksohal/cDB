//
// Created by G on 11/24/2024.
//

#include "ChunkScan.h"

ChunkScan::ChunkScan(Transaction* tx, const std::string& file_name, const Layout& layout, int start, int end)
    : tx(tx),
      file_name(file_name),
      layout(layout),
      start_b_num(start),
      end_b_num(end),
      current_b_num(0),
      current_slot(0) {
    for (int i = start_b_num; i < end_b_num; i++) {
        BlockId const blk(file_name, i);
        buffs.emplace_back(tx, blk, layout);
    }
}

auto ChunkScan::moveToBlock(int blk_num) -> void {
    current_b_num = blk_num;
    rp = std::make_unique<RecordPage>(buffs[current_b_num - start_b_num]);
    current_slot = -1;
}

void ChunkScan::beforeFirst() {
    moveToBlock(start_b_num);
}

auto ChunkScan::next() -> bool {
    current_slot = rp->nextAfter(current_slot);
    while (current_slot < 0) {
        if (current_b_num == end_b_num) {
            return false;
        }
        moveToBlock(rp->block().blockNumber() + 1);
        current_slot = rp->nextAfter(current_slot);
    }
    return true;
}

auto ChunkScan::getInt(const std::string& fld_name) -> int {
    return rp->getInt(current_slot, fld_name);
}

auto ChunkScan::getString(const std::string& fld_name) -> std::string {
    return rp->getString(current_slot, fld_name);
}

auto ChunkScan::getVal(const std::string& fld_name) -> Constant {
    if (layout.schema().type(fld_name) == Schema::INTEGER) {
        return Constant(getInt(fld_name));
    }
    return Constant(getString(fld_name));
}

auto ChunkScan::hasField(const std::string& fld_name) -> bool {
    return layout.schema().hasField(fld_name);
}

void ChunkScan::close() {
    for (int i = 0; i < static_cast<int>(buffs.size()); i++) {
        BlockId blk(file_name, start_b_num + i);
        tx->unpin(blk);
    }
}
