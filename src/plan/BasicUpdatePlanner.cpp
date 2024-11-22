//
// Created by G on 11/22/2024.
//

#include "BasicUpdatePlanner.h"

#include "../query/UpdateScan.h"
#include "SelectPlan.h"
#include "TablePlan.h"

BasicUpdatePlanner::BasicUpdatePlanner(MetadataMgr *mdm) : mdm(mdm) {}

auto BasicUpdatePlanner::executeInsert(InsertData &data, Transaction &tx) -> int {
    std::shared_ptr<Plan> const p = std::make_shared<TablePlan>(&tx, data.tableName(), mdm);
    std::shared_ptr<UpdateScan> const us = std::dynamic_pointer_cast<UpdateScan>(p->open());
    us->insert();
    auto t = data.vals();
    auto it = t.begin();
    for (auto &fld_name : data.fields()) {
        auto val = *it;
        us->setVal(fld_name, val);
        std::advance(it, 1);
    }
    us->close();
    return 1;
}

auto BasicUpdatePlanner::executeDelete(DeleteData &data, Transaction &tx) -> int {
    std::shared_ptr<Plan> p = std::make_shared<TablePlan>(&tx, data.tableName(), mdm);
    p = std::make_shared<SelectPlan>(p, data.pred());
    std::shared_ptr<UpdateScan> const us = std::dynamic_pointer_cast<UpdateScan>(p->open());
    int count = 0;
    while (us->next()) {
        us->remove();
        count++;
    }
    us->close();
    return count;
}

auto BasicUpdatePlanner::executeModify(ModifyData &data, Transaction &tx) -> int {
    std::shared_ptr<Plan> p = std::make_shared<TablePlan>(&tx, data.tableName(), mdm);
    p = std::make_shared<SelectPlan>(p, data.pred());
    std::shared_ptr<UpdateScan> const us = std::dynamic_pointer_cast<UpdateScan>(p->open());
    int count = 0;
    while (us->next()) {
        std::shared_ptr<Scan> const s = us;
        auto val = data.newValue().evaluate(*s);
        us->setVal(data.targetField(), val);
        count++;
    }
    us->close();
    return count;
}

auto BasicUpdatePlanner::executeCreateTable(CreateTableData &data, Transaction &tx) -> int {
    auto s = data.newSchema();
    mdm->createTable(data.tableName(), s, tx);
    return 0;
}

auto BasicUpdatePlanner::executeCreateView(CreateViewData &data, Transaction &tx) -> int {
    mdm->createView(data.viewName(), data.viewDef(), tx);
    return 0;
}

auto BasicUpdatePlanner::executeCreateIndex(CreateIndexData &data, Transaction &tx) -> int {
    mdm->createIndex(data.indexName(), data.tableName(), data.fieldName(), tx);
    return 0;
}
