//
// Created by G on 11/24/2024.
//

#ifndef CDB_TABLEPLANNER_H
#define CDB_TABLEPLANNER_H

#include "../plan/TablePlan.h"
#include "../query/Predicate.h"

class TablePlanner {
public:
    TablePlanner(const std::string& tbl_name, Predicate my_pred, Transaction* tx, MetadataMgr* mdm);
    auto makeSelectPlan() -> std::shared_ptr<Plan>;
    auto makeJoinPlan(const std::shared_ptr<Plan>& current) -> std::shared_ptr<Plan>;
    auto makeProductPlan(const std::shared_ptr<Plan>& current) -> std::shared_ptr<Plan>;

private:
    auto makeIndexSelect() -> std::shared_ptr<Plan>;
    auto makeIndexJoin(const std::shared_ptr<Plan>& current, Schema& curr_sch) -> std::shared_ptr<Plan>;
    auto makeProductJoin(const std::shared_ptr<Plan>& current, Schema& curr_sch) -> std::shared_ptr<Plan>;
    auto addSelectPred(std::shared_ptr<Plan> p) -> std::shared_ptr<Plan>;
    auto addJoinPred(std::shared_ptr<Plan> p, Schema& curr_sch) -> std::shared_ptr<Plan>;
    TablePlan my_plan;
    Predicate my_pred;
    Transaction* tx;
    Schema my_schema;
    std::unordered_map<std::string, IndexInfo> indexes;
};

#endif  // CDB_TABLEPLANNER_H
