//
// Created by G on 10/16/2024.
//
#include "../../src/file/FileManager.h"
#include "gtest/gtest.h"

TEST(file, file_test) {
    const int block_size = 400;

    BlockId const block_id {"testfile", 2};
    std::filesystem::path path = std::filesystem::path {"filetest"};
    FileManager fm {path, block_size};
    int const pos1 = 88;

    Page p1 {fm.blockSize()};
    p1.setString(pos1, "abcdefghijklm");
    int const size = Page::maxLength(static_cast<int>(std::string("abcdefghijklm").size()));
    int const pos2 = pos1 + size;
    p1.setInt(pos2, 345);
    fm.write(block_id, p1);

    Page p2 {fm.blockSize()};
    fm.read(block_id, p2);
    EXPECT_EQ(p2.getInt(pos2), 345);
    EXPECT_EQ(p2.getString(pos1), "abcdefghijklm");
}