//
// Created by G on 11/4/2024.
//
#include <random>

#include "../src/record/Layout.h"
#include "../src/record/RecordPage.h"
#include "../src/record/TableScan.h"
#include "gtest/gtest.h"

namespace {
auto makeFM() -> FileManager {
    std::filesystem::path path = std::filesystem::path {"tx"};
    FileManager fm {path, 400};
    for (int i = 0; i < 10; i++) {
        BlockId const blk {"testfile.tbl", i};
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
    r = std::remove("tx/testfile.tbl");
    EXPECT_EQ(r, 0);
    r = std::remove("tx");
    EXPECT_EQ(r, 0);
}
}  // namespace

TEST(layout, layout_test) {
    Schema sch;
    sch.addIntField("A");
    sch.addStringField("B", 9);
    Layout layout {sch};
    EXPECT_EQ(layout.schema().fields().size(), 2);
    EXPECT_EQ(layout.schema().fields()[0], "A");
    EXPECT_EQ(layout.schema().fields()[1], "B");
    EXPECT_EQ(layout.offset("A"), 4);
    EXPECT_EQ(layout.offset("B"), 8);
}

TEST(table_scan, table_scan_test) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 50);
    auto fm = makeFM();
    auto lm = makeLM(fm);
    BufferManager bm {&fm, &lm, 32};

    Transaction tx(&fm, &lm, &bm);

    Schema sch;
    sch.addIntField("A");
    sch.addStringField("B", 9);

    Layout layout {sch};
    TableScan ts {&tx, "testfile", &layout};
    std::vector<std::pair<int, std::string>> above;
    std::vector<std::pair<int, std::string>> below;
    for (int i = 0; i < 50; i++) {
        if (i == 49) {
            std::cout << "break\n";
        }
        ts.insert();
        int const n = dis(gen);
        ts.setInt("A", n);
        if (n < 25) {
            below.emplace_back(n, "below");
            ts.setString("B", "below");
        } else {
            above.emplace_back(n, "above");
            ts.setString("B", "above");
        }
    }

    std::vector<std::pair<int, std::string>> res;
    ts.beforeFirst();
    while (ts.next()) {
        auto a = ts.getInt("A");
        auto b = ts.getString("B");
        if (a < 25) {
            res.emplace_back(a, b);
            ts.remove();
        }
    }

    EXPECT_EQ(below, res);
    res.clear();

    ts.beforeFirst();
    while (ts.next()) {
        auto a = ts.getInt("A");
        auto b = ts.getString("B");
        res.emplace_back(a, b);
    }

    EXPECT_EQ(above, res);
    ts.close();
    tx.commit();
    cleanUp();
}

TEST(record, record_test) {  // NOLINT
    std::string filename = "testfile";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 50);
    auto fm = makeFM();
    auto lm = makeLM(fm);
    BufferManager bm {&fm, &lm, 8};

    Transaction tx(&fm, &lm, &bm);
    Schema sch;
    sch.addIntField("A");
    sch.addStringField("B", 9);

    Layout layout {sch};
    BlockId blk = tx.append(filename);
    tx.pin(blk);

    RecordPage rp(&tx, blk, &layout);
    rp.format();

    int slot = rp.insertAfter(-1);
    int under_25 = 0;
    int total = 0;
    while (slot >= 0) {
        total++;
        int const n = dis(gen);
        if (n < 25) {
            under_25++;
            rp.setString(slot, "B", "under");
        } else {
            rp.setString(slot, "B", "over");
        }
        rp.setInt(slot, "A", n);
        slot = rp.insertAfter(slot);
    }

    slot = rp.nextAfter(-1);
    int remove_count = 0;
    while (slot >= 0) {
        total--;
        auto a = rp.getInt(slot, "A");
        auto b = rp.getString(slot, "B");
        if (a < 25) {
            remove_count++;
            EXPECT_EQ(b, "under");
            rp.remove(slot);
        } else {
            EXPECT_EQ(b, "over");
        }
        slot = rp.nextAfter(slot);
    }

    EXPECT_EQ(remove_count, under_25);
    EXPECT_EQ(total, 0);
    tx.unpin(blk);
    tx.commit();
    EXPECT_EQ(std::remove("tx/testfile"), 0);
    cleanUp();
}