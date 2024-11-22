//
// Created by G on 11/21/2024.
//

#include "ProductPlan.h"

#include "../query/ProductScan.h"

ProductPlan::ProductPlan(const std::shared_ptr<Plan>& p1, const std::shared_ptr<Plan>& p2) : p1(p1), p2(p2) {
    auto s = p1->schema();
    schema_m.addAll(s);
    s = p2->schema();
    schema_m.addAll(s);
}

auto ProductPlan::open() -> std::shared_ptr<Scan> {
    auto s1 = p1->open();
    auto s2 = p2->open();
    std::shared_ptr<Scan> ret = std::make_shared<ProductScan>(s1, s2);
    return ret;
}

auto ProductPlan::blocksAccessed() -> int {
    return p1->blocksAccessed() + (p1->recordsOutput() * p1->recordsOutput());
}

auto ProductPlan::recordsOutput() -> int {
    return p1->recordsOutput() + p2->recordsOutput();
}

auto ProductPlan::distinctValues() -> int {
    return p1->distinctValues();
}

auto ProductPlan::schema() -> Schema {
    return schema_m;
}
