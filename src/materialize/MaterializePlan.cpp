//
// Created by G on 11/23/2024.
//

#include "MaterializePlan.h"

#include <cmath>
#include <utility>

#include "TempTable.h"

MaterializePlan::MaterializePlan(Transaction* tx, std::shared_ptr<Plan> src_plan)
    : src_plan(std::move(src_plan)), tx(tx) {}

auto MaterializePlan::open() -> std::shared_ptr<Scan> {
    auto sch = src_plan->schema();
    TempTable temp(tx, sch);
    auto src = src_plan->open();
    auto dest = temp.open();
    while (src->next()) {
        dest.insert();
        for (auto& fld_name : sch.fields()) {
            dest.setVal(fld_name, src->getVal(fld_name));
        }
    }
    src->close();
    dest.beforeFirst();
    return std::make_shared<TableScan>(dest);
}

auto MaterializePlan::blocksAccessed() -> int {
    auto s = src_plan->schema();
    Layout const layout(s);
    double const rpb = static_cast<double>(tx->blockSize()) / layout.slotSize();
    return static_cast<int>(std::ceil(src_plan->recordsOutput() / rpb));
}

auto MaterializePlan::recordsOutput() -> int {
    return src_plan->recordsOutput();
}

auto MaterializePlan::distinctValues() -> int {
    return src_plan->distinctValues();
}

auto MaterializePlan::schema() -> Schema {
    return src_plan->schema();
}
