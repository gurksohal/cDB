//
// Created by G on 11/23/2024.
//

#include "GroupByScan.h"

#include <utility>

GroupByScan::GroupByScan(std::shared_ptr<Scan> s, std::vector<std::string> group_fields,
                         std::vector<std::shared_ptr<AggregationFn>> agg_fns)
    : s(std::move(s)), group_fields(std::move(group_fields)), agg_fns(std::move(agg_fns)), more_groups(false) {
    this->s->beforeFirst();
    auto t = this->s->next();
    more_groups = t;
}

void GroupByScan::beforeFirst() {
    this->s->beforeFirst();
    more_groups = this->s->next();
}

auto GroupByScan::next() -> bool {
    if (!more_groups) {
        return false;
    }

    for (auto& fn : agg_fns) {
        fn->processFirst(s);
    }
    group_val = std::make_unique<GroupValue>(s.get(), group_fields);
    more_groups = s->next();
    while (more_groups) {
        more_groups = s->next();
        auto gv = std::make_unique<GroupValue>(s.get(), group_fields);
        if (group_val != gv) {
            break;
        }
        for (auto& fn : agg_fns) {
            fn->processFirst(s);
        }
    }
    return true;
}

auto GroupByScan::getInt(const std::string& fld_name) -> int {
    return getVal(fld_name).asInt();
}

auto GroupByScan::getString(const std::string& fld_name) -> std::string {
    return getVal(fld_name).asString();
}

auto GroupByScan::getVal(const std::string& fld_name) -> Constant {
    auto non_const_fld = fld_name;
    if (std::count(group_fields.begin(), group_fields.end(), non_const_fld) != 0) {
        return group_val->getVal(non_const_fld);
    }
    for (auto& fn : agg_fns) {
        if (fn->fieldName() == fld_name) {
            return fn->value();
        }
    }
    throw std::runtime_error("field " + fld_name + " not found");
}

auto GroupByScan::hasField(const std::string& fld_name) -> bool {
    if (std::count(group_fields.begin(), group_fields.end(), fld_name) != 0) {
        return true;
    }

    return std::any_of(agg_fns.begin(), agg_fns.end(),
                       [&fld_name](const std::shared_ptr<AggregationFn>& fn) { return fn->fieldName() == fld_name; });
}

void GroupByScan::close() {
    s->close();
}
