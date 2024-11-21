//
// Created by G on 11/20/2024.
//

#ifndef CDB_QUERYDATA_H
#define CDB_QUERYDATA_H

#include <ostream>

#include "../query/Predicate.h"

class QueryData {
public:
    QueryData(std::vector<std::string> fields, std::vector<std::string> tables, Predicate pred);
    auto fields() -> std::vector<std::string>;
    auto tables() -> std::vector<std::string>;
    auto pred() -> Predicate;
    friend auto operator<<(std::ostream& os, const QueryData& data) -> std::ostream&;

private:
    std::vector<std::string> fields_m;
    std::vector<std::string> tables_m;
    Predicate pred_m;
};

#endif  // CDB_QUERYDATA_H
