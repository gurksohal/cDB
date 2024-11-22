//
// Created by G on 11/19/2024.
//

#include "Term.h"

#include <climits>
#include <utility>

auto operator<<(std::ostream& os, const Term& term) -> std::ostream& {
    os << term.lhs << "=" << term.rhs;
    return os;
}

Term::Term(Expression left, Expression right) : lhs(std::move(left)), rhs(std::move(right)) {}

auto Term::isSatisfied(Scan& scan) -> bool {
    Constant const lhs_val = lhs.evaluate(scan);
    Constant const rhs_val = rhs.evaluate(scan);
    return rhs_val == lhs_val;
}

auto Term::reductionFactor(Plan& p) -> int {
    if (lhs.isFldName() && rhs.isFldName()) {
        return std::max(p.distinctValues(), p.distinctValues());
    }

    if (lhs.isFldName()) {
        return p.distinctValues();
    }

    if (rhs.isFldName()) {
        return p.distinctValues();
    }

    if (lhs.asConstant() == rhs.asConstant()) {
        return -1;
    }

    return INT_MAX;
}

auto Term::equateWithConstant(const std::string& fld_name) -> std::unique_ptr<Constant> {
    if (lhs.isFldName() && lhs.asFieldName() == fld_name && !rhs.isFldName()) {
        return std::make_unique<Constant>(rhs.asConstant());
    }
    if (rhs.isFldName() && rhs.asFieldName() == fld_name && !lhs.isFldName()) {
        return std::make_unique<Constant>(lhs.asConstant());
    }
    return {};
}

auto Term::equateWithField(const std::string& fld_name) -> std::string {
    if (lhs.isFldName() && lhs.asFieldName() == fld_name && rhs.isFldName()) {
        return rhs.asFieldName();
    }

    if (rhs.isFldName() && rhs.asFieldName() == fld_name && lhs.isFldName()) {
        return lhs.asFieldName();
    }

    return {};
}

auto Term::appliesTo(const Schema& sch) -> bool {
    return lhs.appliesTo(sch) && rhs.appliesTo(sch);
}
