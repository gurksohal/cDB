//
// Created by G on 11/20/2024.
//

#ifndef CDB_CREATEVIEWDATA_H
#define CDB_CREATEVIEWDATA_H
#include <sstream>
#include <string>

#include "QueryData.h"

class CreateViewData {
public:
    CreateViewData(std::string view_name, QueryData qry_data)
        : view_name(std::move(view_name)), qry_data(std::move(qry_data)) {}

    auto viewName() -> std::string {
        return view_name;
    }

    auto viewDef() -> std::string {
        std::ostringstream oss;
        oss << qry_data;
        return oss.str();
    }

private:
    std::string view_name;
    QueryData qry_data;
};
#endif  // CDB_CREATEVIEWDATA_H
