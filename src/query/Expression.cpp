//
// Created by G on 11/19/2024.
//

#include "Expression.h"

#include <utility>

auto operator<<(std::ostream& os, const Expression& expression) -> std::ostream& {
    if (!expression.fld_name.empty()) {
        os << expression.fld_name;
    } else {
        os << expression.val;
    }
    return os;
}

auto Expression::asConstant() -> Constant {
    return val;
}

Expression::Expression(Constant val) : val(std::move(val)) {}

Expression::Expression(std::string fld_name) : val(-1), fld_name(std::move(fld_name)) {}

auto Expression::evaluate(Scan& s) -> Constant {
    if (!fld_name.empty()) {
        return s.getVal(fld_name);
    }
    return val;
}

auto Expression::isFldName() -> bool {
    return !fld_name.empty();
}

auto Expression::asFieldName() -> std::string {
    return fld_name;
}

auto Expression::appliesTo(const Schema& sch) -> bool {
    if (!fld_name.empty()) {
        return true;
    }

    return sch.hasField(fld_name);
}
