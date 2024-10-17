//
// Created by G on 10/16/2024.
//

#ifndef CDB_PAGE_H
#define CDB_PAGE_H

#include <string>
#include <vector>

class Page {
public:
    explicit Page(int block_size);
    explicit Page(std::vector<std::byte> bytes);
    auto getInt(int offset) const -> int;
    auto getBytes(int offset) const -> std::vector<std::byte>;
    auto getString(int offset) const -> std::string;
    auto setInt(int offset, int val) -> void;
    auto setBytes(int offset, std::vector<std::byte>& val) -> void;
    auto setString(int offset, const std::string& val) -> void;
    auto replaceData(std::vector<std::byte>& bytes) -> void;
    auto getData() -> std::vector<std::byte>&;

    static auto maxLength(int str_len) -> int;

private:
    std::vector<std::byte> data;
};

#endif  // CDB_PAGE_H
