//
// Created by G on 11/22/2024.
//

#ifndef CDB_PLANNER_H
#define CDB_PLANNER_H

#include "QueryPlanner.h"
#include "UpdatePlanner.h"

class Planner {
public:
    Planner(std::shared_ptr<QueryPlanner> q_planner, std::shared_ptr<UpdatePlanner> u_planner);
    auto createQueryPlan(std::string& qry, Transaction& tx) -> std::shared_ptr<Plan>;
    auto executeUpdate(std::string& cmd, Transaction& tx) -> int;

private:
    std::shared_ptr<QueryPlanner> q_planner;
    std::shared_ptr<UpdatePlanner> u_planner;
};

#endif  // CDB_PLANNER_H
