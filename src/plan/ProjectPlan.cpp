//
// Created by G on 11/21/2024.
//

#include "ProjectPlan.h"

#include "../query/ProjectScan.h"

ProjectPlan::ProjectPlan(const std::shared_ptr<Plan> &p, std::vector<std::string> &fld_list) : p(p) {
    for (auto &f : fld_list) {
        sch.add(f, p->schema());
    }
}

auto ProjectPlan::open() -> std::shared_ptr<Scan> {
    auto s = p->open();
    std::shared_ptr<Scan> ret = std::make_shared<ProjectScan>(s, sch.fields());
    return ret;
}

auto ProjectPlan::blocksAccessed() -> int {
    return p->blocksAccessed();
}

auto ProjectPlan::recordsOutput() -> int {
    return p->recordsOutput();
}

auto ProjectPlan::distinctValues() -> int {
    return p->distinctValues();
}

auto ProjectPlan::schema() -> Schema {
    return sch;
}
