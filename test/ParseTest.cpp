//
// Created by G on 11/20/2024.
//
#include "../src/parse/Parser.h"
#include "gtest/gtest.h"

TEST(parse, parse_test) {
    std::string select_str = "select t from c where a=b";
    Parser p(select_str);
    EXPECT_NO_FATAL_FAILURE(p.query());
    std::string update_str = "update t set a=b where c=1";
    p = Parser(update_str);
    EXPECT_NO_FATAL_FAILURE(p.updateCmd());
}

TEST(parse, parse_actions_test) {
    std::string select_str = "select t from c where a=b";
    std::string update_str = "update t set a=b where c=1";
    Parser p(select_str);
    std::ostringstream oss;
    oss << p.query();
    EXPECT_EQ(oss.str(), select_str);

    p = Parser(update_str);
    EXPECT_TRUE(std::holds_alternative<ModifyData>(p.updateCmd()));
}