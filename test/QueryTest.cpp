//
// Created by G on 11/19/2024.
//
#include "../src/file/FileManager.h"
#include "../src/log/LogManager.h"
#include "../src/query/Expression.h"
#include "../src/query/Predicate.h"
#include "../src/query/ProductScan.h"
#include "../src/query/ProjectScan.h"
#include "../src/query/SelectScan.h"
#include "../src/record/Layout.h"
#include "../src/record/TableScan.h"
#include "gtest/gtest.h"

namespace {
const char* const FOLDER = "query";

auto makeFM() -> FileManager {
    std::filesystem::path path = std::filesystem::path {FOLDER};
    FileManager fm {path, 400};
    for (int i = 0; i < 10; i++) {
        BlockId const blk {"testfile", i};
        Page p(fm.blockSize());
        fm.write(blk, p);
    }
    std::ofstream const file(path / "T.tbl");
    std::ofstream const file2(path / "T1.tbl");
    std::ofstream const file3(path / "T2.tbl");
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

TEST(query, scan1_test) {
    auto fm = makeFM();
    auto lm = makeLM(fm);
    BufferManager bm {&fm, &lm, 8};
    Transaction tx(&fm, &lm, &bm);

    Schema sch1;
    sch1.addIntField("A");
    sch1.addStringField("B", 9);
    Layout const layout(sch1);
    std::unique_ptr<UpdateScan> s1 = std::make_unique<TableScan>(&tx, "T", layout);

    s1->beforeFirst();
    int const n = 200;
    for (int i = 0; i < n; i++) {
        s1->insert();
        s1->setInt("A", i);
        s1->setString("B", "rec" + std::to_string(i));
    }
    s1->close();

    std::shared_ptr<Scan> const s2 = std::make_shared<TableScan>(&tx, "T", layout);
    Constant const c(10);
    Term const t(Expression("A"), Expression(c));
    Predicate pred(t);
    std::shared_ptr<Scan> const s3 = std::make_shared<SelectScan>(s2, pred);

    std::vector<std::string> fields({"B"});
    std::unique_ptr<Scan> s4 = std::make_unique<ProjectScan>(s3, fields);
    int count = 0;
    while (s4->next()) {
        count++;
    }
    s4->close();
    tx.commit();
    EXPECT_EQ(count, 1);
    cleanUp();
}

TEST(query, scan2_test) {
    auto fm = makeFM();
    auto lm = makeLM(fm);
    BufferManager bm {&fm, &lm, 8};
    Transaction tx(&fm, &lm, &bm);

    Schema sch1;
    sch1.addIntField("A");
    sch1.addStringField("B", 9);
    Layout const layout1(sch1);
    std::shared_ptr<UpdateScan> const us1 = std::make_shared<TableScan>(&tx, "T1", layout1);
    us1->beforeFirst();
    int const n = 200;
    for (int i = 0; i < n; i++) {
        us1->insert();
        us1->setInt("A", i);
        us1->setString("B", "bbb" + std::to_string(i));
    }
    us1->close();

    Schema sch2;
    sch2.addIntField("C");
    sch2.addStringField("D", 9);
    Layout const layout2(sch2);
    std::shared_ptr<UpdateScan> const us2 = std::make_shared<TableScan>(&tx, "T2", layout2);
    us2->beforeFirst();
    for (int i = 0; i < n; i++) {
        us2->insert();
        us2->setInt("C", n - i - 1);
        us2->setString("D", "ddd" + std::to_string(n - i - 1));
    }
    us2->close();

    std::shared_ptr<Scan> s1 = std::make_shared<TableScan>(&tx, "T1", layout1);
    std::shared_ptr<Scan> s2 = std::make_shared<TableScan>(&tx, "T2", layout2);
    std::shared_ptr<Scan> const s3 = std::make_shared<ProductScan>(s1, s2);

    Term const t(Expression("A"), Expression("C"));
    Predicate pred(t);

    std::shared_ptr<Scan> const s4 = std::make_shared<SelectScan>(s3, pred);
    std::vector<std::string> c {"B", "D"};
    std::shared_ptr<Scan> const s5 = std::make_shared<ProjectScan>(s4, c);

    int count = 0;
    while (s5->next()) {
        auto a = s5->getString("B");
        auto b = s5->getString("D");
        EXPECT_EQ(a, "bbb" + std::to_string(count));
        EXPECT_EQ(b, "ddd" + std::to_string(count));
        count++;
    }
    EXPECT_EQ(count, 200);
    cleanUp();
}

TEST(query, product_test) {
    auto fm = makeFM();
    auto lm = makeLM(fm);
    BufferManager bm {&fm, &lm, 8};
    Transaction tx(&fm, &lm, &bm);

    Schema sch1;
    sch1.addIntField("A");
    sch1.addStringField("B", 9);
    Layout const layout1(sch1);
    std::shared_ptr<TableScan> const ts1 = std::make_shared<TableScan>(&tx, "T1", layout1);

    Schema sch2;
    sch2.addIntField("C");
    sch2.addStringField("D", 9);
    Layout const layout2(sch2);
    std::shared_ptr<TableScan> const ts2 = std::make_shared<TableScan>(&tx, "T2", layout2);

    ts1->beforeFirst();
    int const n = 200;
    for (int i = 0; i < n; i++) {
        ts1->insert();
        ts1->setInt("A", i);
        ts1->setString("B", "aaa" + std::to_string(i));
    }
    ts1->close();

    ts2->beforeFirst();
    for (int i = 0; i < n; i++) {
        ts2->insert();
        ts2->setInt("C", n - i - 1);
        ts2->setString("D", "bbb" + std::to_string(n - i - 1));
    }
    ts2->close();

    std::shared_ptr<Scan> s1 = std::make_shared<TableScan>(&tx, "T1", layout1);
    std::shared_ptr<Scan> s2 = std::make_shared<TableScan>(&tx, "T2", layout2);
    std::shared_ptr<Scan> const s3 = std::make_shared<ProductScan>(s1, s2);

    int count = 0;
    int freq = 0;
    while (s3->next()) {
        auto a = s3->getString("B");
        EXPECT_EQ(a, "aaa" + std::to_string(freq));
        count++;
        if (count % 200 == 0) {
            freq++;
        }
    }

    EXPECT_EQ(count, 40000);
    s3->close();
    tx.commit();
    cleanUp();
}