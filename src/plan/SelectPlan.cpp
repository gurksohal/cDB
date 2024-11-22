//
// Created by G on 11/21/2024.
//

#include "SelectPlan.h"

#include <utility>

#include "../query/SelectScan.h"

SelectPlan::SelectPlan(std::shared_ptr<Plan> p, Predicate pred) : p(std::move(p)), pred(std::move(pred)) {}

auto SelectPlan::open() -> std::shared_ptr<Scan> {
    auto s = p->open();
    std::shared_ptr<Scan> ret = std::make_shared<SelectScan>(s, pred);
    return ret;
}

auto SelectPlan::blocksAccessed() -> int {
    return p->blocksAccessed();
}

auto SelectPlan::recordsOutput() -> int {
    return p->recordsOutput() / pred.reductionFactor(*p);
}

auto SelectPlan::distinctValues() -> int {
    return p->distinctValues();
}

auto SelectPlan::schema() -> Schema {
    return p->schema();
}
