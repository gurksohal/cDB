//
// Created by G on 11/24/2024.
//

#include "MultiBufferProductPlan.h"

#include <utility>

#include "../materialize/MaterializePlan.h"
#include "MultiBufferProductScan.h"

MultiBufferProductPlan::MultiBufferProductPlan(Transaction *tx, std::shared_ptr<Plan> lhs, std::shared_ptr<Plan> rhs)
    : tx(tx), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

auto MultiBufferProductPlan::copyRecordsFrom(Plan *p) -> std::unique_ptr<TempTable> {
    auto src = p->open();
    auto sch = p->schema();
    std::unique_ptr<TempTable> t = std::make_unique<TempTable>(tx, sch);
    auto dest = t->open();
    while (src->next()) {
        dest.insert();
        for (auto &fld_name : sch.fields()) {
            dest.setVal(fld_name, src->getVal(fld_name));
        }
    }

    src->close();
    dest.close();
    return t;
}

auto MultiBufferProductPlan::open() -> std::shared_ptr<Scan> {
    auto left_scan = lhs->open();
    auto tt = copyRecordsFrom(rhs.get());
    return std::make_shared<MultiBufferProductScan>(tx, left_scan, tt->tableName(), tt->getLayout());
}

auto MultiBufferProductPlan::blocksAccessed() -> int {
    int const avail = tx->availableBuffs();
    int const size = MaterializePlan(tx, rhs).blocksAccessed();
    int const num_chunks = size / avail;
    return rhs->blocksAccessed() + (lhs->blocksAccessed() * num_chunks);
}

auto MultiBufferProductPlan::recordsOutput() -> int {
    return lhs->recordsOutput() * rhs->recordsOutput();
}

auto MultiBufferProductPlan::distinctValues() -> int {
    return lhs->distinctValues();
}

auto MultiBufferProductPlan::schema() -> Schema {
    return schema_m;
}
