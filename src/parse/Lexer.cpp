//
// Created by G on 11/20/2024.
//

#include "Lexer.h"

#include <vector>

Lexer::Lexer(const std::string& s) : stream(s), t_type(0), n_val(0) {
    initKeywords();
    nextToken();
}

auto Lexer::matchDelim(char d) const -> bool {
    return t_type == d;
}

auto Lexer::matchIntConstant() const -> bool {
    return t_type == TT_NUMBER;
}

auto Lexer::matchStringConstant() const -> bool {
    return t_type == TT_STRING;
}

auto Lexer::matchKeyword(const std::string& w) -> bool {
    return t_type == TT_WORD && s_val == w;
}

auto Lexer::matchId() -> bool {
    return t_type == TT_WORD && keywords.find(s_val) == keywords.end();
}

auto Lexer::eatIntConstant() -> int {
    if (!matchIntConstant()) {
        throw std::runtime_error("bad syntax");
    }
    int const i = n_val;
    nextToken();
    return i;
}

auto Lexer::eatStringConstant() -> std::string {
    if (!matchStringConstant()) {
        throw std::runtime_error("bad syntax");
    }
    std::string s = s_val;
    nextToken();
    return s;
}

auto Lexer::eatKeyword(const std::string& w) -> void {
    if (!matchKeyword(w)) {
        throw std::runtime_error("bad syntax");
    }
    nextToken();
}

auto Lexer::eatId() -> std::string {
    if (!matchId()) {
        throw std::runtime_error("bad syntax");
    }
    std::string s = s_val;
    nextToken();
    return s;
}

auto Lexer::initKeywords() -> void {
    std::vector<std::string> const kw = {"select", "from",    "where",  "and", "insert", "into",
                                         "values", "delete",  "update", "set", "create", "table",
                                         "int",    "varchar", "view",   "as",  "index",  "on"};
    keywords = std::unordered_set<std::string>(std::begin(kw), std::end(kw));
}

auto Lexer::nextToken() -> void {  // NOLINT
    s_val.clear();
    n_val = 0;
    t_type = 0;

    char c = 0;
    while (stream.get(c)) {
        if (std::isspace(c) != 0) {
            continue;
        }

        if (std::isdigit(c) != 0) {
            // Read integer constant
            std::string num_str(1, c);
            while (stream.peek() != EOF && (std::isdigit(stream.peek()) != 0)) {
                stream.get(c);
                num_str += c;
            }
            n_val = std::stoi(num_str);
            t_type = TT_NUMBER;
            return;
        }

        if ((std::isalpha(c) != 0) || c == '_') {
            std::string word(1, c);
            while (stream.peek() != EOF && ((std::isalnum(stream.peek()) != 0) || stream.peek() == '_')) {
                stream.get(c);
                word += c;
            }
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            s_val = word;
            t_type = TT_WORD;
            return;
        }

        if (c == '\'') {
            std::string str;
            while (stream.get(c)) {
                if (c == '\'') {
                    break;
                }
                str += c;
            }
            s_val = str;
            t_type = TT_STRING;
            return;
        }

        t_type = static_cast<unsigned char>(c);
        return;
    }
    t_type = TT_EOF;
}

auto Lexer::eatDelim(char d) -> void {
    if (!matchDelim(d)) {
        throw std::runtime_error("bad syntax");
    }
    nextToken();
}
