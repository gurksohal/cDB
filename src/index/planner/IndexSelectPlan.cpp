//
// Created by G on 11/23/2024.
//

#include "IndexSelectPlan.h"

#include <utility>

#include "../../record/TableScan.h"
#include "../query/IndexSelectScan.h"

IndexSelectPlan::IndexSelectPlan(std::shared_ptr<Plan> p, IndexInfo *ii, std::shared_ptr<Constant> val)
    : p(std::move(p)), ii(ii), val(std::move(val)) {}

auto IndexSelectPlan::open() -> std::shared_ptr<Scan> {
    auto ts = std::dynamic_pointer_cast<TableScan>(p->open());
    auto idx = ii->open();
    return std::make_shared<IndexSelectScan>(ts, idx, val);
}

auto IndexSelectPlan::blocksAccessed() -> int {
    return ii->blocksAccessed() + recordsOutput();
}

auto IndexSelectPlan::recordsOutput() -> int {
    return ii->recordsOutput();
}

auto IndexSelectPlan::distinctValues() -> int {
    return ii->distinctValues("");
}

auto IndexSelectPlan::schema() -> Schema {
    return p->schema();
}
