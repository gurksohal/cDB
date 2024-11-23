//
// Created by G on 11/23/2024.
//

#include "GroupByPlan.h"

#include "GroupByScan.h"
#include "SortPlan.h"

GroupByPlan::GroupByPlan(Transaction& tx, std::shared_ptr<Plan> p, std::vector<std::string> group_fields,
                         std::vector<std::shared_ptr<AggregationFn>> agg_fns)
    : group_fields(std::move(group_fields)), agg_fns(std::move(agg_fns)) {
    this->p = std::make_shared<SortPlan>(&tx, p, group_fields);
    for (auto& fld_name : group_fields) {
        sch.add(fld_name, p->schema());
    }
    for (auto& fn : this->agg_fns) {
        sch.addIntField(fn->fieldName());
    }
}

auto GroupByPlan::open() -> std::shared_ptr<Scan> {
    auto s = p->open();
    return std::make_shared<GroupByScan>(s, group_fields, agg_fns);
}

auto GroupByPlan::blocksAccessed() -> int {
    return p->blocksAccessed();
}

auto GroupByPlan::recordsOutput() -> int {
    int num_groups = 1;
    for (auto& fld_name : group_fields) {
        num_groups *= p->distinctValues();
    }
    return num_groups;
}

auto GroupByPlan::distinctValues() -> int {
    return p->distinctValues();
}

auto GroupByPlan::schema() -> Schema {
    return sch;
}
