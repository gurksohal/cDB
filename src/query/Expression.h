//
// Created by G on 11/19/2024.
//

#ifndef CDB_EXPRESSION_H
#define CDB_EXPRESSION_H

#include <ostream>
#include <string>

#include "../record/Schema.h"
#include "Constant.h"
#include "Scan.h"

class Expression {
public:
    explicit Expression(Constant val);
    explicit Expression(std::string fld_name);
    auto evaluate(Scan& s) -> Constant;
    auto isFldName() -> bool;
    auto asConstant() -> Constant;
    auto asFieldName() -> std::string;
    auto appliesTo(const Schema& sch) -> bool;
    friend auto operator<<(std::ostream& os, const Expression& expression) -> std::ostream&;

private:
    Constant val;
    std::string fld_name;
};

#endif  // CDB_EXPRESSION_H
