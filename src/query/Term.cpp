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

auto Term::reductionFactor() -> int {  // TODO: After plan, pass in plan object
    throw std::runtime_error("to impl" + std::to_string(static_cast<int>(lhs.isFldName())));
    //    std::string lhs_name;
    //    std::string rhs_name;
    //    if (lhs.isFieldName() && rhs.isFieldName()) {
    //        lhs_name = lhs.asFieldName();
    //        rhs_name = rhs.asFieldName();
    //        return std::max(p.distinctValues(lhsName),
    //                        p.distinctValues(rhsName));
    //    }
    //
    //    if (lhs.isFieldName()) {
    //        lhs_name = lhs.asFieldName();
    //        return p.distinctValues(lhs_name);
    //    }
    //
    //    if (rhs.isFieldName()) {
    //        rhs_name = rhs.asFieldName();
    //        return p.distinctValues(rhs_name);
    //    }
    //
    //    if (lhs.asConstant() == rhs.asConstant()) {
    //        return -1;
    //    }
    //
    //    return INT_MAX;
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
