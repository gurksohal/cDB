//
// Created by G on 11/22/2024.
//
#include "../src/file/FileManager.h"
#include "../src/log/LogManager.h"
#include "../src/metadata/MetadataMgr.h"
#include "../src/plan/BasicQueryPlanner.h"
#include "../src/plan/BasicUpdatePlanner.h"
#include "../src/plan/BetterQueryPlanner.h"
#include "../src/plan/Planner.h"
#include "../src/plan/TablePlan.h"
#include "gtest/gtest.h"

namespace {
const char* const FOLDER = "plan";

auto makeFM() -> FileManager {
    std::filesystem::path path = std::filesystem::path {FOLDER};
    FileManager fm {path, 400};
    for (int i = 0; i < 10; i++) {
        BlockId const blk {"testfile", i};
        Page p(fm.blockSize());
        fm.write(blk, p);
    }
    std::ofstream const f1(path / "tblcat.tbl");
    std::ofstream const f2(path / "fldcat.tbl");
    std::ofstream const f3(path / "viewcat.tbl");
    std::ofstream const f4(path / "idxcat.tbl");
    std::ofstream const f5(path / "T1.tbl");
    std::ofstream const f6(path / "T2.tbl");
    return {path, 400};
}

auto makeLM(FileManager& fm) -> LogManager {
    std::string const log_file_path = std::string(FOLDER) + "/logtestfile";
    std::ofstream const file(log_file_path);
    return {&fm, "logtestfile"};
}

auto cleanUp() -> void {
    for (const auto& entry : std::filesystem::directory_iterator(FOLDER)) {
        const auto* p = entry.path().c_str();
        int const r = std::remove(p);
        EXPECT_EQ(r, 0);
    }
    int const r = std::remove(FOLDER);
    EXPECT_EQ(r, 0);
}
}  // namespace

TEST(plan, planner1_test) {
    auto fm = makeFM();
    auto lm = makeLM(fm);
    BufferManager bm {&fm, &lm, 8};
    Transaction tx(&fm, &lm, &bm);
    MetadataMgr mdm(true, tx);

    Planner planner(std::make_shared<BetterQueryPlanner>(&mdm), std::make_shared<BasicUpdatePlanner>(&mdm));
    std::string cmd = "create table T1(A int, B varchar(9))";
    planner.executeUpdate(cmd, tx);

    int const n = 200;
    for (int i = 0; i < n; i++) {
        std::string const a = std::to_string(i);
        std::string const b = "rec" + std::to_string(i);
        cmd = std::string("insert into T1(A,B) values(" + a + ", '" + b + "')");  // NOLINT
        planner.executeUpdate(cmd, tx);
    }

    std::string qry = "select B from T1 where A=10";
    auto p = planner.createQueryPlan(qry, tx);
    auto s = p->open();
    int count = 0;
    while (s->next()) {
        EXPECT_EQ(s->getString("b"), "rec10");
        count++;
    }

    EXPECT_EQ(count, 1);
    s->close();
    tx.commit();
    cleanUp();
}

TEST(plan, planner2_test) {
    auto fm = makeFM();
    auto lm = makeLM(fm);
    BufferManager bm {&fm, &lm, 8};
    Transaction tx(&fm, &lm, &bm);
    MetadataMgr mdm(true, tx);

    Planner planner(std::make_shared<BasicQueryPlanner>(&mdm), std::make_shared<BasicUpdatePlanner>(&mdm));
    std::string cmd = "create table T1(A int, B varchar(9))";
    planner.executeUpdate(cmd, tx);

    int const n = 200;
    for (int i = 0; i < n; i++) {
        std::string const a = std::to_string(i);
        std::string const b = "bbb" + std::to_string(i);
        cmd = std::string("insert into T1(A,B) values(" + a + ", '" + b + "')");  // NOLINT
        planner.executeUpdate(cmd, tx);
    }

    cmd = "create table T2(C int, D varchar(9))";
    planner.executeUpdate(cmd, tx);

    for (int i = 0; i < n; i++) {
        std::string const a = std::to_string(n - i - 1);
        std::string const b = "ddd" + std::to_string(n - i - 1);
        cmd = std::string("insert into T2(C,D) values(" + a + ", '" + b + "')");  // NOLINT
        planner.executeUpdate(cmd, tx);
    }

    std::string qry = "select B,D from T1,T2 where A=C";
    auto p = planner.createQueryPlan(qry, tx);
    auto s = p->open();
    int count = 0;
    while (s->next()) {
        auto b = s->getString("b");
        auto d = s->getString("d");
        EXPECT_EQ(b, "bbb" + std::to_string(count));
        EXPECT_EQ(d, "ddd" + std::to_string(count));
        count++;
    }

    EXPECT_EQ(count, 200);
    s->close();
    tx.commit();
    cleanUp();
}