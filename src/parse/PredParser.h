//
// Created by G on 11/20/2024.
//

#ifndef CDB_PREDPARSER_H
#define CDB_PREDPARSER_H
#include "Lexer.h"

class PredParser {
public:
    explicit PredParser(const std::string& s) : lex(s) {};

    auto field() -> std::string {
        return lex.eatId();
    }

    auto constant() -> void {
        if (lex.matchStringConstant()) {
            lex.eatStringConstant();
        } else {
            lex.eatIntConstant();
        }
    }

    auto expression() -> void {
        if (lex.matchId()) {
            field();
        } else {
            constant();
        }
    }

    auto term() -> void {
        expression();
        lex.eatDelim('=');
        expression();
    }

    auto predicate() -> void {
        term();
        while (lex.matchKeyword("and")) {
            lex.eatKeyword("and");
            term();
        }
    }

private:
    Lexer lex;
};
#endif  // CDB_PREDPARSER_H
