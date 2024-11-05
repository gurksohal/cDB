//
// Created by G on 11/4/2024.
//

#include "RID.h"

RID::RID(int blk_num, int slot) : blk_num(blk_num), slot_m(slot) {}

auto RID::blockNumber() const -> int {
    return blk_num;
}

auto RID::slot() const -> int {
    return slot_m;
}

auto RID::operator==(const RID& rhs) const -> bool {
    return blk_num == rhs.blk_num && slot_m == rhs.slot_m;
}

auto RID::operator!=(const RID& rhs) const -> bool {
    return !(rhs == *this);
}

auto operator<<(std::ostream& os, const RID& rid) -> std::ostream& {
    os << "[blk_num: " << rid.blk_num << " slot_m: " << rid.slot_m << "]";
    return os;
}
