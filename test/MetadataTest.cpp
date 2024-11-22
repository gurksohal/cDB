//
// Created by G on 11/15/2024.
//
#include "../src/buffer/BufferManager.h"
#include "../src/metadata/MetadataMgr.h"
#include "../src/record/TableScan.h"
#include "gtest/gtest.h"

namespace {
auto makeFM() -> FileManager {
    std::filesystem::path path = std::filesystem::path {"metadata"};
    FileManager fm {path, 400};
    for (int i = 0; i < 10; i++) {
        BlockId const blk {"testfile", i};
        Page p(fm.blockSize());
        fm.write(blk, p);
    }
    std::ofstream const file(path / "tblcat.tbl");
    std::ofstream const file2(path / "fldcat.tbl");
    std::ofstream const file3(path / "viewcat.tbl");
    std::ofstream const file4(path / "MyTable.tbl");
    std::ofstream const file5(path / "idxcat.tbl");
    for (int i = 0; i < 10; i++) {
        BlockId const blk {"MyTable.tbl", i};
        Page p(fm.blockSize());
        fm.write(blk, p);
    }
    for (int i = 0; i < 10; i++) {
        BlockId const blk {"idxcat.tbl", i};
        Page p(fm.blockSize());
        fm.write(blk, p);
    }
    return {path, 400};
}

auto makeLM(FileManager& fm) -> LogManager {
    std::string const log_file_path = "metadata/logtestfile";
    std::ofstream const file(log_file_path);
    return {&fm, "logtestfile"};
}

auto cleanUp() -> void {
    std::string const folder = "metadata";
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        const auto* p = entry.path().c_str();
        int const r = std::remove(p);
        EXPECT_EQ(r, 0);
    }
    int const r = std::remove("metadata");
    EXPECT_EQ(r, 0);
}
}  // namespace

TEST(metadata, metadata_mgr_test) {  // NOLINT
    auto fm = makeFM();
    auto lm = makeLM(fm);
    BufferManager bm {&fm, &lm, 8};
    Transaction tx(&fm, &lm, &bm);
    MetadataMgr mdm(true, tx);

    Schema sch;
    sch.addIntField("A");
    sch.addStringField("B", 9);

    mdm.createTable("MyTable", sch, tx);
    Layout layout = mdm.getLayout("MyTable", tx);
    int const size = layout.slotSize();
    Schema const sch2 = layout.schema();
    EXPECT_EQ(size, 21);

    TableScan ts(&tx, "MyTable", layout);
    for (int i = 0; i < 50; i++) {
        ts.insert();
        ts.setInt("A", i);
        ts.setString("B", "rec" + std::to_string(i));
    }

    StatInfo const si = mdm.getStatInfo("MyTable", layout, tx);
    EXPECT_EQ(si.blockAccessed(), 3);
    EXPECT_EQ(si.recordsOutput(), 50);
    EXPECT_EQ(si.distinctValues(), 17);

    std::string const view_def = "select B from MyTable where A = 1";
    mdm.createView("viewA", view_def, tx);
    auto v = mdm.getViewDef("viewA", tx);
    EXPECT_EQ(v, view_def);

    mdm.createIndex("indexA", "MyTable", "A", tx);
    mdm.createIndex("indexB", "MyTable", "B", tx);
    auto idx_map = mdm.getIndexInfo("MyTable", tx);

    IndexInfo ii = idx_map["A"];
    // EXPECT_EQ(ii.blocksAccessed(), 0); //TODO: uncomment after index
    EXPECT_EQ(ii.recordsOutput(), 2);
    EXPECT_EQ(ii.distinctValues("A"), 1);
    EXPECT_EQ(ii.distinctValues("B"), 17);

    ii = idx_map["B"];
    // EXPECT_EQ(ii.blocksAccessed(), 0); //TODO: uncomment after index
    EXPECT_EQ(ii.recordsOutput(), 2);
    EXPECT_EQ(ii.distinctValues("A"), 17);
    EXPECT_EQ(ii.distinctValues("B"), 1);
    tx.commit();
    cleanUp();
}

TEST(metadata, table_mgr_test) {
    auto fm = makeFM();
    auto lm = makeLM(fm);
    BufferManager bm {&fm, &lm, 8};
    Transaction tx(&fm, &lm, &bm);
    TableMgr tm(true, tx);

    Schema sch;
    sch.addIntField("A");
    sch.addStringField("B", 9);
    tm.createTable("MyTable", sch, tx);
    Layout layout = tm.getLayout("MyTable", tx);
    int const size = layout.slotSize();
    Schema sch2 = layout.schema();
    EXPECT_EQ(size, 21);
    EXPECT_EQ(sch2.fields().size(), 2);
    EXPECT_EQ(sch2.type(sch2.fields()[0]), Schema::INTEGER);
    EXPECT_EQ(sch2.type(sch2.fields()[1]), Schema::VARCHAR);
    EXPECT_EQ(sch2.length(sch2.fields()[1]), 9);
    cleanUp();
}