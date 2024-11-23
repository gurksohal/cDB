//
// Created by G on 11/23/2024.
//

#include "IndexUpdatePlanner.h"

#include "../../plan/SelectPlan.h"
#include "../../plan/TablePlan.h"
#include "../../query/UpdateScan.h"

IndexUpdatePlanner::IndexUpdatePlanner(MetadataMgr *mdm) : mdm(mdm) {}

auto IndexUpdatePlanner::executeInsert(InsertData &data, Transaction &tx) -> int {
    auto tbl_name = data.tableName();
    auto p = std::make_shared<TablePlan>(&tx, tbl_name, mdm);

    auto s = std::dynamic_pointer_cast<UpdateScan>(p->open());
    s->insert();
    auto rid = s->getRid();

    auto indexes = mdm->getIndexInfo(tbl_name, tx);
    auto t = data.vals();
    auto val_iter = t.begin();
    for (auto &fld_name : data.fields()) {
        auto val = *val_iter;
        std::advance(val_iter, 1);
        s->setVal(fld_name, val);

        if (indexes.contains(fld_name)) {
            auto idx = indexes[fld_name].open();
            idx->insert(val, rid);
            idx->close();
        }
    }
    s->close();
    return 1;
}

auto IndexUpdatePlanner::executeDelete(DeleteData &data, Transaction &tx) -> int {
    auto tbl_name = data.tableName();
    std::shared_ptr<Plan> p = std::make_shared<TablePlan>(&tx, tbl_name, mdm);
    p = std::make_shared<SelectPlan>(p, data.pred());
    auto indexes = mdm->getIndexInfo(tbl_name, tx);

    auto s = std::dynamic_pointer_cast<UpdateScan>(p->open());
    int count = 0;
    while (s->next()) {
        RID rid = s->getRid();
        for (auto &[fld_name, indexInfo] : indexes) {
            Constant val = s->getVal(fld_name);
            auto idx = indexInfo.open();
            idx->remove(val, rid);
            idx->close();
        }

        s->remove();
        count++;
    }
    s->close();

    return count;
}

auto IndexUpdatePlanner::executeModify(ModifyData &data, Transaction &tx) -> int {
    std::string const tbl_name = data.tableName();
    std::string const fld_name = data.targetField();
    std::shared_ptr<Plan> p = std::make_shared<TablePlan>(&tx, tbl_name, mdm);
    p = std::make_shared<SelectPlan>(p, data.pred());

    auto t = mdm->getIndexInfo(tbl_name, tx);
    std::shared_ptr<Index> idx;
    if (t.contains(fld_name)) {
        idx = t[fld_name].open();
    }

    auto s = std::dynamic_pointer_cast<UpdateScan>(p->open());
    int count = 0;
    while (s->next()) {
        auto new_val = data.newValue().evaluate(*s);
        auto old_val = s->getVal(fld_name);
        s->setVal(data.targetField(), new_val);

        if (idx) {
            auto rid = s->getRid();
            idx->remove(old_val, rid);
            idx->insert(new_val, rid);
        }
        count++;
    }
    if (idx) {
        idx->close();
    }
    s->close();
    return count;
}

auto IndexUpdatePlanner::executeCreateTable(CreateTableData &data, Transaction &tx) -> int {
    auto s = data.newSchema();
    mdm->createTable(data.tableName(), s, tx);
    return 0;
}

auto IndexUpdatePlanner::executeCreateView(CreateViewData &data, Transaction &tx) -> int {
    mdm->createView(data.viewName(), data.viewDef(), tx);
    return 0;
}

auto IndexUpdatePlanner::executeCreateIndex(CreateIndexData &data, Transaction &tx) -> int {
    mdm->createIndex(data.indexName(), data.tableName(), data.fieldName(), tx);
    return 0;
}
