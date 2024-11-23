//
// Created by G on 11/24/2024.
//

#ifndef CDB_SORTPLAN_H
#define CDB_SORTPLAN_H

#include "../plan/Plan.h"
#include "../tx/Transaction.h"
#include "RecordComparator.h"
#include "TempTable.h"

class SortPlan : public Plan {
public:
    SortPlan(Transaction* tx, std::shared_ptr<Plan>& p, std::vector<std::string>& sort_fields);
    auto open() -> std::shared_ptr<Scan> override;
    auto blocksAccessed() -> int override;
    auto recordsOutput() -> int override;
    auto distinctValues() -> int override;
    auto schema() -> Schema override;

private:
    auto splitIntoRuns(TableScan* src) -> std::vector<std::unique_ptr<TempTable>>;
    auto doAMergeIteration(std::vector<std::unique_ptr<TempTable>>& runs) -> std::vector<std::unique_ptr<TempTable>>;
    auto mergeTwoRuns(TempTable* p1, TempTable* p2) -> std::unique_ptr<TempTable>;
    auto copy(TableScan& src, TableScan& dest) -> bool;
    Transaction* tx;
    std::shared_ptr<Plan> p;
    Schema sch;
    RecordComparator comp;
};

#endif  // CDB_SORTPLAN_H
