//
// Created by G on 11/19/2024.
//

#ifndef CDB_PREDICATE_H
#define CDB_PREDICATE_H

#include <ostream>
#include <vector>

#include "../plan/Plan.h"
#include "Term.h"

class Predicate {
public:
    explicit Predicate();
    explicit Predicate(Term t);
    auto conjoinWith(Predicate& pred) -> void;
    auto isSatisfied(Scan& scan) -> bool;
    auto reductionFactor(Plan& p) -> int;
    auto selectSubPred(const Schema& sch) -> std::unique_ptr<Predicate>;
    auto joinSubPred(Schema& sch1, Schema& sch2) -> std::unique_ptr<Predicate>;
    auto equatesWithConstant(const std::string& fld_name) -> std::unique_ptr<Constant>;
    auto equatesWithField(const std::string& fld_name) -> std::string;
    friend auto operator<<(std::ostream& os, const Predicate& predicate) -> std::ostream&;

private:
    std::vector<Term> terms;
};

#endif  // CDB_PREDICATE_H
