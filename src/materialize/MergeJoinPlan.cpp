//
// Created by G on 11/24/2024.
//

#include "MergeJoinPlan.h"

#include <utility>

#include "MergeJoinScan.h"
#include "SortPlan.h"
#include "SortScan.h"

MergeJoinPlan::MergeJoinPlan(Transaction& tx, std::shared_ptr<Plan> p1, std::shared_ptr<Plan> p2, std::string fld_name1,
                             std::string fld_name2)
    : fld_name1(std::move(fld_name1)), fld_name2(std::move(fld_name2)) {
    std::vector<std::string> temp = {this->fld_name1};
    p1 = std::make_shared<SortPlan>(&tx, p1, temp);
    temp = {this->fld_name2};
    p2 = std::make_shared<SortPlan>(&tx, p2, temp);
    auto s = p1->schema();
    sch.addAll(s);
    s = p2->schema();
    sch.addAll(s);
}

auto MergeJoinPlan::open() -> std::shared_ptr<Scan> {
    auto s1 = p1->open();
    auto s2 = std::dynamic_pointer_cast<SortScan>(p2->open());
    return std::make_shared<MergeJoinScan>(s1, s2, fld_name1, fld_name2);
}

auto MergeJoinPlan::blocksAccessed() -> int {
    return p1->blocksAccessed() + p2->blocksAccessed();
}

auto MergeJoinPlan::recordsOutput() -> int {
    return (p1->recordsOutput() * p2->recordsOutput()) * p1->distinctValues();
}

auto MergeJoinPlan::distinctValues() -> int {
    return p1->distinctValues();
}

auto MergeJoinPlan::schema() -> Schema {
    return sch;
}
