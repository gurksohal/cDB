//
// Created by G on 11/19/2024.
//

#include "Predicate.h"

auto operator<<(std::ostream& os, const Predicate& predicate) -> std::ostream& {
    if (predicate.terms.empty()) {
        os << "";
    } else {
        bool first = true;
        for (auto i : predicate.terms) {
            if (first) {
                first = false;
                os << i;
            } else {
                os << " and " << i;
            }
        }
    }
    return os;
}

Predicate::Predicate() = default;

Predicate::Predicate(Term t) : terms({t}) {}

auto Predicate::conjoinWith(Predicate& pred) -> void {
    for (auto& t : pred.terms) {
        terms.push_back(t);
    }
}

auto Predicate::isSatisfied(Scan& scan) -> bool {
    for (auto& t : terms) {
        if (!t.isSatisfied(scan)) {
            return false;
        }
    }
    return true;
}

auto Predicate::reductionFactor(Plan& p) -> int {
    int factor = 1;
    for (auto& t : terms) {
        factor *= t.reductionFactor(p);
    }
    return factor;
}

auto Predicate::selectSubPred(const Schema& sch) -> std::unique_ptr<Predicate> {
    Predicate result;
    for (auto& t : terms) {
        if (t.appliesTo(sch)) {
            result.terms.push_back(t);
        }
    }
    if (result.terms.empty()) {
        return {};
    }

    return std::make_unique<Predicate>(result);
}

auto Predicate::joinSubPred(Schema& sch1, Schema& sch2) -> std::unique_ptr<Predicate> {
    Predicate result;
    Schema new_sch;
    new_sch.addAll(sch1);
    new_sch.addAll(sch2);
    for (auto& t : terms) {
        if (!t.appliesTo(sch1) && !t.appliesTo(sch2) && !t.appliesTo(new_sch)) {
            result.terms.push_back(t);
        }
    }

    if (result.terms.empty()) {
        return {};
    }

    return std::make_unique<Predicate>(result);
}

auto Predicate::equatesWithConstant(const std::string& fld_name) -> std::unique_ptr<Constant> {
    for (auto& t : terms) {
        std::unique_ptr<Constant> c = t.equateWithConstant(fld_name);
        if (c != nullptr) {
            return c;
        }
    }

    return {};
}

auto Predicate::equatesWithField(const std::string& fld_name) -> std::string {
    for (auto& t : terms) {
        auto s = t.equateWithField(fld_name);
        if (!s.empty()) {
            return s;
        }
    }
    return {};
}
