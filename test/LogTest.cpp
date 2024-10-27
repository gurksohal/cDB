//
// Created by G on 10/26/2024.
//
#include "../src/log/LogManager.h"
#include "gtest/gtest.h"

namespace {
auto createRecord(const std::string& s, int n) -> std::vector<std::byte> {
    int const s_pos = 0;
    int const n_pos = s_pos + Page::maxLength(static_cast<int>(s.length()));
    std::vector<std::byte> const res(n_pos + static_cast<int>(sizeof(int)));
    Page p {res};
    p.setString(s_pos, s);
    p.setInt(n_pos, n);
    return p.getData();
}

auto createRecords(int start, int end, LogManager& lm) -> std::vector<std::vector<std::byte>> {
    std::vector<std::vector<std::byte>> res;
    for (int i = start; i <= end; i++) {
        auto rec = createRecord("record" + std::to_string(i), i + 100);
        lm.append(rec);
        res.push_back(rec);
    }

    return res;
}

void cmp(std::vector<std::vector<std::byte>> expected, LogManager& lm) {
    std::reverse(expected.begin(), expected.end());
    auto it = lm.iterator();
    for (const auto& i : expected) {
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ(i, it.next());
    }
}
}  // namespace

TEST(log, log_test) {
    std::filesystem::path path = std::filesystem::path {"logtest"};
    FileManager fm {path, 400};
    std::string const log_file_path = "logtest/logtestfile";

    // create log file
    std::ofstream const file(log_file_path);

    LogManager log_manager(&fm, "logtestfile");
    auto it = log_manager.iterator();
    EXPECT_FALSE(it.hasNext());

    auto added = createRecords(1, 35, log_manager);
    cmp(added, log_manager);

    added = createRecords(36, 70, log_manager);
    cmp(added, log_manager);

    // clean up files
    auto r = std::remove("logtest/logtestfile");
    EXPECT_EQ(r, 0);
    r = std::remove("logtest");
    EXPECT_EQ(r, 0);
}