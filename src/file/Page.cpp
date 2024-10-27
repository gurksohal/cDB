//
// Created by G on 10/16/2024.
//

#include "Page.h"

#include <cstddef>
#include <cstring>

Page::Page(int block_size) : data(block_size) {}

Page::Page(std::vector<std::byte> bytes) : data(std::move(bytes)) {}

auto Page::getInt(int offset) const -> int {
    int res = 0;
    std::memcpy(&res, &(data[offset]), sizeof(int));
    return res;
}

auto Page::getBytes(int offset) const -> std::vector<std::byte> {
    const int length = getInt(offset);
    std::vector<std::byte> buffer(length);
    auto start = data.begin() + offset + sizeof(int);
    auto end = start + length;
    std::copy(start, end, buffer.begin());
    return buffer;
}

auto Page::getString(int offset) const -> std::string {
    auto bytes = getBytes(offset);
    std::string res;
    for (auto& b : bytes) {
        res += std::to_integer<char>(b);
    }
    return res;
}

auto Page::setInt(int offset, int val) -> void {
    std::memcpy(&data[offset], &val, sizeof(int));
}

auto Page::setBytes(int offset, std::vector<std::byte>& val) -> void {
    setInt(offset, static_cast<int>(val.size()));
    auto start = data.begin() + offset + sizeof(int);
    std::copy(val.begin(), val.end(), start);
}

auto Page::setString(int offset, const std::string& val) -> void {
    std::vector<std::byte> bytes(val.size());
    int i = 0;
    for (char const c : val) {
        bytes[i++] = (static_cast<std::byte>(c));
    }
    setBytes(offset, bytes);
}

auto Page::replaceData(std::vector<std::byte>& bytes) -> void {
    data = std::move(bytes);
}

auto Page::maxLength(int str_len) -> int {
    return static_cast<int>(sizeof(int)) + str_len;
}

auto Page::getData() -> std::vector<std::byte>& {
    return data;
}
