//
// Created by G on 11/4/2024.
//

#ifndef CDB_RID_H
#define CDB_RID_H

#include <ostream>

class RID {
public:
    RID(int blk_num, int slot);
    auto blockNumber() const -> int;
    auto slot() const -> int;
    auto operator==(const RID& rhs) const -> bool;
    auto operator!=(const RID& rhs) const -> bool;
    friend auto operator<<(std::ostream& os, const RID& rid) -> std::ostream&;

private:
    int blk_num;
    int slot_m;
};

#endif  // CDB_RID_H
