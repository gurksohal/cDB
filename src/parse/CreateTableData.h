//
// Created by G on 11/20/2024.
//

#ifndef CDB_CREATETABLEDATA_H
#define CDB_CREATETABLEDATA_H
#include <utility>

#include "../record/Schema.h"

class CreateTableData {
public:
    CreateTableData(std::string tbl_name, Schema sch) : tbl_name(std::move(tbl_name)), sch(std::move(sch)) {}

    auto tableName() -> std::string {
        return tbl_name;
    }

    auto newSchema() -> Schema {
        return sch;
    }

private:
    std::string tbl_name;
    Schema sch;
};
#endif  // CDB_CREATETABLEDATA_H
