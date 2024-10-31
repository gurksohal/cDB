//
// Created by G on 10/30/2024.
//
#include "../src/buffer/BufferManager.h"
#include "../src/tx/Transaction.h"
#include "gtest/gtest.h"

namespace {
auto makeFM() -> FileManager {
    std::filesystem::path path = std::filesystem::path {"concurrency"};
    FileManager fm {path, 400};
    for (int i = 0; i < 10; i++) {
        BlockId const blk {"testfile", i};
        Page p(fm.blockSize());
        fm.write(blk, p);
    }
    return {path, 400};
}

auto makeLM(FileManager& fm) -> LogManager {
    std::string const log_file_path = "concurrency/logtestfile";
    std::ofstream const file(log_file_path);
    return {&fm, "logtestfile"};
}

auto cleanUp() -> void {
    auto r = std::remove("concurrency/logtestfile");
    EXPECT_EQ(r, 0);
    r = std::remove("concurrency/testfile");
    EXPECT_EQ(r, 0);
    r = std::remove("concurrency");
    EXPECT_EQ(r, 0);
}

auto a(Transaction& tx) {
    BlockId blk1("testfile", 1);
    BlockId blk2("testfile", 2);
    tx.pin(blk1);
    tx.pin(blk2);
    int const f = tx.getInt(blk1, 0);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(f, tx.getInt(blk1, 0));
    tx.getInt(blk2, 0);
    tx.commit();
}

auto b(Transaction& tx) {
    BlockId blk1("testfile", 1);
    BlockId blk2("testfile", 2);
    tx.pin(blk1);
    tx.pin(blk2);

    tx.setInt(blk2, 0, 5, false);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(tx.getInt(blk2, 0), 5);
    tx.getInt(blk2, 0);
    tx.commit();
}

auto c(Transaction& tx) {
    BlockId blk1("testfile", 1);
    BlockId blk2("testfile", 2);
    tx.pin(blk1);
    tx.pin(blk2);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    tx.setInt(blk1, 0, 0, false);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    tx.getInt(blk2, 0);
    tx.commit();
}
}  // namespace

TEST(concurrency, concurrency_test) {
    auto fm = makeFM();
    auto lm = makeLM(fm);
    BufferManager bm {&fm, &lm, 8};
    Transaction tx1(&fm, &lm, &bm);
    Transaction tx2(&fm, &lm, &bm);
    Transaction tx3(&fm, &lm, &bm);
    std::thread t1([&]() { a(tx1); });
    std::thread t2([&]() { b(tx2); });
    std::thread t3([&]() { c(tx3); });
    t1.join();
    t2.join();
    t3.join();
    cleanUp();
}