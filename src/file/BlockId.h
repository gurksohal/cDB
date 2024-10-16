//
// Created by G on 10/16/2024.
//

#ifndef CDB_BLOCKID_H
#define CDB_BLOCKID_H

#include <ostream>
#include <string>

class BlockId {
public:
    BlockId(std::string& filename, int block_num);
    auto filename() const -> std::string_view;
    auto blockNumber() const -> int;
    auto operator==(const BlockId& rhs) const -> bool;
    auto operator!=(const BlockId& rhs) const -> bool;
    friend auto operator<<(std::ostream& os, const BlockId& id) -> std::ostream&;

private:
    std::string file_name;
    int block_num;
};

#endif  // CDB_BLOCKID_H
