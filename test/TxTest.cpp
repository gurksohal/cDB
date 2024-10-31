//
// Created by G on 10/30/2024.
//
#include "../src/buffer/BufferManager.h"
#include "../src/tx/Transaction.h"
#include "gtest/gtest.h"

namespace {
auto makeFM() -> FileManager {
    std::filesystem::path path = std::filesystem::path {"tx"};
    FileManager fm {path, 400};
    for (int i = 0; i < 10; i++) {
        BlockId const blk {"testfile", i};
        Page p(fm.blockSize());
        fm.write(blk, p);
    }
    return {path, 400};
}

auto makeLM(FileManager& fm) -> LogManager {
    std::string const log_file_path = "tx/logtestfile";
    std::ofstream const file(log_file_path);
    return {&fm, "logtestfile"};
}

auto cleanUp() -> void {
    auto r = std::remove("tx/logtestfile");
    EXPECT_EQ(r, 0);
    r = std::remove("tx/testfile");
    EXPECT_EQ(r, 0);
    r = std::remove("tx");
    EXPECT_EQ(r, 0);
}
}  // namespace

TEST(tx, tx_test) {
    auto fm = makeFM();
    auto lm = makeLM(fm);
    BufferManager bm {&fm, &lm, 8};

    Transaction tx1(&fm, &lm, &bm);
    BlockId blk("testfile", 1);
    tx1.pin(blk);
    tx1.setInt(blk, 80, 1, false);
    tx1.setString(blk, 40, "one", false);
    tx1.commit();

    Transaction tx2(&fm, &lm, &bm);
    tx2.pin(blk);
    auto i_val = tx2.getInt(blk, 80);
    auto s_val = tx2.getString(blk, 40);
    EXPECT_EQ(i_val, 1);
    EXPECT_EQ(s_val, "one");

    auto new_i_val = i_val + 1;
    auto new_s_val = s_val + "!";
    tx2.setInt(blk, 80, new_i_val, true);
    tx2.setString(blk, 40, new_s_val, true);
    tx2.commit();
    Transaction tx3(&fm, &lm, &bm);
    tx3.pin(blk);
    EXPECT_EQ(new_i_val, tx3.getInt(blk, 80));
    EXPECT_EQ(new_s_val, tx3.getString(blk, 40));

    tx3.setInt(blk, 80, 9999, true);
    auto pre_roll = tx3.getInt(blk, 80);
    EXPECT_EQ(pre_roll, 9999);
    tx3.rollback();

    Transaction tx4(&fm, &lm, &bm);
    tx4.pin(blk);
    auto post_roll = tx4.getInt(blk, 80);
    EXPECT_EQ(post_roll, new_i_val);
    tx4.commit();
    cleanUp();
}
