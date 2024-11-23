//
// Created by G on 11/23/2024.
//

#ifndef CDB_TEMPTABLE_H
#define CDB_TEMPTABLE_H

#include "../query/UpdateScan.h"
#include "../record/Layout.h"
#include "../record/TableScan.h"
#include "../tx/Transaction.h"

class TempTable {
public:
    TempTable(Transaction* tx, Schema sch);
    auto open() -> TableScan;
    auto tableName() -> std::string;
    auto getLayout() -> Layout;

private:
    static int next_table_num;
    auto nextTableName() -> std::string;
    std::mutex mutex;
    Transaction* tx;
    std::string tbl_name;
    Layout layout;
};

#endif  // CDB_TEMPTABLE_H
