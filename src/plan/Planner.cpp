//
// Created by G on 11/22/2024.
//

#include "Planner.h"

#include <utility>

#include "../parse/Parser.h"

Planner::Planner(std::shared_ptr<QueryPlanner> q_planner, std::shared_ptr<UpdatePlanner> u_planner)
    : q_planner(std::move(q_planner)), u_planner(std::move(u_planner)) {}

auto Planner::createQueryPlan(std::string& qry, Transaction& tx) -> std::shared_ptr<Plan> {
    Parser parser(qry);
    auto data = parser.query();
    return q_planner->createPlan(data, tx);
}

auto Planner::executeUpdate(std::string& cmd, Transaction& tx) -> int {
    Parser parser(cmd);
    auto data = parser.updateCmd();
    if (std::holds_alternative<InsertData>(data)) {
        return u_planner->executeInsert(std::get<InsertData>(data), tx);
    }
    if (std::holds_alternative<DeleteData>(data)) {
        return u_planner->executeDelete(std::get<DeleteData>(data), tx);
    }
    if (std::holds_alternative<ModifyData>(data)) {
        return u_planner->executeModify(std::get<ModifyData>(data), tx);
    }
    if (std::holds_alternative<CreateTableData>(data)) {
        return u_planner->executeCreateTable(std::get<CreateTableData>(data), tx);
    }
    if (std::holds_alternative<CreateViewData>(data)) {
        return u_planner->executeCreateView(std::get<CreateViewData>(data), tx);
    }
    if (std::holds_alternative<CreateIndexData>(data)) {
        return u_planner->executeCreateIndex(std::get<CreateIndexData>(data), tx);
    }
    return 0;
}
