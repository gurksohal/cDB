//
// Created by G on 11/24/2024.
//

#include "SortPlan.h"

#include "MaterializePlan.h"
#include "SortScan.h"

SortPlan::SortPlan(Transaction* tx, std::shared_ptr<Plan>& p, std::vector<std::string>& sort_fields)
    : tx(tx), p(p), sch(p->schema()), comp(sort_fields) {}

auto SortPlan::open() -> std::shared_ptr<Scan> {
    auto src = std::dynamic_pointer_cast<TableScan>(p->open());
    std::vector<std::unique_ptr<TempTable>> runs = splitIntoRuns(src.get());
    src->close();
    while (runs.size() > 2) {
        runs = doAMergeIteration(runs);
    }
    return std::make_shared<SortScan>(runs, comp);
}

auto SortPlan::blocksAccessed() -> int {
    auto mp = std::make_shared<MaterializePlan>(tx, p);
    return mp->blocksAccessed();
}

auto SortPlan::recordsOutput() -> int {
    return p->recordsOutput();
}

auto SortPlan::distinctValues() -> int {
    return p->distinctValues();
}

auto SortPlan::schema() -> Schema {
    return sch;
}

auto SortPlan::splitIntoRuns(TableScan* src) -> std::vector<std::unique_ptr<TempTable>> {
    std::vector<std::unique_ptr<TempTable>> temps;
    src->beforeFirst();
    if (!src->next()) {
        return temps;
    }

    auto current_temp = std::make_unique<TempTable>(tx, sch);
    temps.push_back(std::move(current_temp));
    auto current_scan = temps.back()->open();

    while (copy(*src, current_scan)) {
        if (comp.compare(src, &current_scan) < 0) {
            current_scan.close();
            auto new_temp = std::make_unique<TempTable>(tx, sch);
            temps.push_back(std::move(new_temp));
            current_scan = temps.back()->open();
        }
    }

    current_scan.close();
    return temps;
}

auto SortPlan::doAMergeIteration(std::vector<std::unique_ptr<TempTable>>& runs)
    -> std::vector<std::unique_ptr<TempTable>> {
    std::vector<std::unique_ptr<TempTable>> result;
    while (runs.size() > 1) {
        auto p1 = std::move(result.front());
        result.erase(runs.begin());
        auto p2 = std::move(result.front());
        result.erase(runs.begin());
        result.push_back(mergeTwoRuns(p1.get(), p2.get()));
    }
    if (runs.size() == 1) {
        result.push_back(std::move(runs.front()));
    }
    return result;
}

auto SortPlan::mergeTwoRuns(TempTable* p1, TempTable* p2) -> std::unique_ptr<TempTable> {
    auto src1 = p1->open();
    auto src2 = p2->open();
    auto result = std::make_unique<TempTable>(tx, sch);
    auto dest = result->open();

    bool has_more1 = src1.next();
    bool has_more2 = src2.next();
    while (has_more1 && has_more2) {
        if (comp.compare(&src1, &src2) < 0) {
            has_more1 = copy(src1, dest);
        } else {
            has_more2 = copy(src2, dest);
        }
    }

    if (has_more1) {
        while (has_more1) {
            has_more1 = copy(src1, dest);
        }
    } else {
        while (has_more2) {
            has_more2 = copy(src2, dest);
        }
    }

    src1.close();
    src2.close();
    dest.close();
    return result;
}

auto SortPlan::copy(TableScan& src, TableScan& dest) -> bool {
    dest.insert();
    for (auto& fld_name : sch.fields()) {
        dest.setVal(fld_name, src.getVal(fld_name));
    }
    return src.next();
}
