//
// Created by G on 11/19/2024.
//

#ifndef CDB_TERM_H
#define CDB_TERM_H

#include <memory>
#include <ostream>

#include "../record/Schema.h"
#include "Expression.h"
#include "Scan.h"

class Term {
public:
    Term(Expression left, Expression right);
    auto isSatisfied(Scan& scan) -> bool;
    auto reductionFactor() -> int;  // TODO: After plan
    auto equateWithConstant(const std::string& fld_name) -> std::unique_ptr<Constant>;
    auto equateWithField(const std::string& fld_name) -> std::string;
    auto appliesTo(const Schema& sch) -> bool;
    friend auto operator<<(std::ostream& os, const Term& term) -> std::ostream&;

private:
    Expression lhs;
    Expression rhs;
};

#endif  // CDB_TERM_H
