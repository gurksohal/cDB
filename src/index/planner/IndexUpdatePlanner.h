//
// Created by G on 11/23/2024.
//

#ifndef CDB_INDEXUPDATEPLANNER_H
#define CDB_INDEXUPDATEPLANNER_H

#include "../../metadata/MetadataMgr.h"
#include "../../plan/UpdatePlanner.h"

class IndexUpdatePlanner : public UpdatePlanner {
public:
    explicit IndexUpdatePlanner(MetadataMgr* mdm);
    auto executeInsert(InsertData& data, Transaction& tx) -> int override;
    auto executeDelete(DeleteData& data, Transaction& tx) -> int override;
    auto executeModify(ModifyData& data, Transaction& tx) -> int override;
    auto executeCreateTable(CreateTableData& data, Transaction& tx) -> int override;
    auto executeCreateView(CreateViewData& data, Transaction& tx) -> int override;
    auto executeCreateIndex(CreateIndexData& data, Transaction& tx) -> int override;

private:
    MetadataMgr* mdm;
};

#endif  // CDB_INDEXUPDATEPLANNER_H
