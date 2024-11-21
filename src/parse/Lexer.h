//
// Created by G on 11/20/2024.
//

#ifndef CDB_LEXER_H
#define CDB_LEXER_H

#include <sstream>
#include <string>
#include <unordered_set>

class Lexer {
public:
    explicit Lexer(const std::string& s);
    auto matchDelim(char d) const -> bool;
    auto matchIntConstant() const -> bool;
    auto matchStringConstant() const -> bool;
    auto matchKeyword(const std::string& w) -> bool;
    auto matchId() -> bool;
    auto eatDelim(char d) -> void;
    auto eatIntConstant() -> int;
    auto eatStringConstant() -> std::string;
    auto eatKeyword(const std::string& w) -> void;
    auto eatId() -> std::string;

private:
    auto initKeywords() -> void;
    auto nextToken() -> void;
    std::unordered_set<std::string> keywords;
    std::istringstream stream;
    int t_type;
    std::string s_val;
    int n_val;
    static const int TT_EOF = -1;
    static const int TT_WORD = -2;
    static const int TT_NUMBER = -3;
    static const int TT_STRING = -4;
};

#endif  // CDB_LEXER_H
