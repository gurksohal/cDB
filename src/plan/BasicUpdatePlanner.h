//
// Created by G on 11/22/2024.
//

#ifndef CDB_BASICUPDATEPLANNER_H
#define CDB_BASICUPDATEPLANNER_H

#include "../metadata/MetadataMgr.h"
#include "UpdatePlanner.h"

class BasicUpdatePlanner : public UpdatePlanner {
public:
    explicit BasicUpdatePlanner(MetadataMgr* mdm);
    auto executeInsert(InsertData& data, Transaction& tx) -> int override;
    auto executeDelete(DeleteData& data, Transaction& tx) -> int override;
    auto executeModify(ModifyData& data, Transaction& tx) -> int override;
    auto executeCreateTable(CreateTableData& data, Transaction& tx) -> int override;
    auto executeCreateView(CreateViewData& data, Transaction& tx) -> int override;
    auto executeCreateIndex(CreateIndexData& data, Transaction& tx) -> int override;

private:
    MetadataMgr* mdm;
};

#endif  // CDB_BASICUPDATEPLANNER_H
