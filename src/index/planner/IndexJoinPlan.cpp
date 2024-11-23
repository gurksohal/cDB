//
// Created by G on 11/23/2024.
//

#include "IndexJoinPlan.h"

#include <utility>

#include "../../record/TableScan.h"
#include "../query/IndexJoinScan.h"

IndexJoinPlan::IndexJoinPlan(std::shared_ptr<Plan> p1, std::shared_ptr<Plan> p2, IndexInfo *ii, std::string join_field)
    : p1(std::move(p1)), p2(std::move(p2)), ii(ii), join_field(std::move(join_field)) {
    auto s = this->p1->schema();
    sch.addAll(s);
    s = this->p2->schema();
    sch.addAll(s);
}

auto IndexJoinPlan::open() -> std::shared_ptr<Scan> {
    auto s = p1->open();
    std::shared_ptr<TableScan> const ts = std::dynamic_pointer_cast<TableScan>(p2->open());
    auto idx = ii->open();
    return std::make_shared<IndexJoinScan>(s, idx, join_field, ts);
}

auto IndexJoinPlan::blocksAccessed() -> int {
    return p1->blocksAccessed() + (p1->recordsOutput() * ii->blocksAccessed()) + recordsOutput();
}

auto IndexJoinPlan::recordsOutput() -> int {
    return p1->recordsOutput() * ii->recordsOutput();
}

auto IndexJoinPlan::distinctValues() -> int {
    return p1->distinctValues();
}

auto IndexJoinPlan::schema() -> Schema {
    return sch;
}
