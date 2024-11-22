//
// Created by G on 11/22/2024.
//

#ifndef CDB_UPDATEPLANNER_H
#define CDB_UPDATEPLANNER_H
#include "../parse/CreateIndexData.h"
#include "../parse/CreateTableData.h"
#include "../parse/CreateViewData.h"
#include "../parse/DeletaData.h"
#include "../parse/InsertData.h"
#include "../parse/ModifyData.h"
#include "../tx/Transaction.h"

class UpdatePlanner {
public:
    UpdatePlanner() = default;
    UpdatePlanner(UpdatePlanner&) = default;
    UpdatePlanner(UpdatePlanner&&) = default;
    auto operator=(const UpdatePlanner&) -> UpdatePlanner& = default;
    auto operator=(UpdatePlanner&&) -> UpdatePlanner& = default;
    virtual ~UpdatePlanner() = default;
    virtual auto executeInsert(InsertData& data, Transaction& tx) -> int = 0;
    virtual auto executeDelete(DeleteData& data, Transaction& tx) -> int = 0;
    virtual auto executeModify(ModifyData& data, Transaction& tx) -> int = 0;
    virtual auto executeCreateTable(CreateTableData& data, Transaction& tx) -> int = 0;
    virtual auto executeCreateView(CreateViewData& data, Transaction& tx) -> int = 0;
    virtual auto executeCreateIndex(CreateIndexData& data, Transaction& tx) -> int = 0;
};
#endif  // CDB_UPDATEPLANNER_H
