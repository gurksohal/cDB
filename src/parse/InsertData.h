//
// Created by G on 11/20/2024.
//

#ifndef CDB_INSERTDATA_H
#define CDB_INSERTDATA_H

#include <string>
#include <utility>
#include <vector>

#include "../query/Constant.h"

class InsertData {
public:
    InsertData(std::string tbl_name, std::vector<std::string> fields, std::vector<Constant> vals)
        : tbl_name(std::move(tbl_name)), fields_m(std::move(fields)), vals_m(std::move(vals)) {}

    auto tableName() -> std::string {
        return tbl_name;
    }

    auto fields() -> std::vector<std::string> {
        return fields_m;
    }

    auto vals() -> std::vector<Constant> {
        return vals_m;
    }

private:
    std::string tbl_name;
    std::vector<std::string> fields_m;
    std::vector<Constant> vals_m;
};
#endif  // CDB_INSERTDATA_H
