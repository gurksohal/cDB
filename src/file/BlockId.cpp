//
// Created by G on 10/16/2024.
//

#include "BlockId.h"

BlockId::BlockId(std::string& filename, int block_num) : file_name(filename), block_num(block_num) {}

auto BlockId::filename() const -> std::string_view {
    return file_name;
}

auto BlockId::blockNumber() const -> int {
    return block_num;
}

auto BlockId::operator==(const BlockId& rhs) const -> bool {
    return file_name == rhs.file_name && block_num == rhs.block_num;
}

auto BlockId::operator!=(const BlockId& rhs) const -> bool {
    return !(rhs == *this);
}

auto operator<<(std::ostream& os, const BlockId& id) -> std::ostream& {
    os << "[file_name: " << id.file_name << " block_num: " << id.block_num << "]";
    return os;
}
