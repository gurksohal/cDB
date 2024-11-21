//
// Created by G on 11/20/2024.
//

#include "QueryData.h"

#include <sstream>
#include <utility>

auto operator<<(std::ostream& os, const QueryData& data) -> std::ostream& {
    std::string res = "select ";
    for (const auto& fld_name : data.fields_m) {
        res += fld_name + ", ";
    }
    res = res.substr(0, res.size() - 2);
    res += " from ";
    for (const auto& tbl_name : data.tables_m) {
        res += tbl_name + ", ";
    }
    res = res.substr(0, res.size() - 2);
    std::ostringstream oss;
    oss << data.pred_m;
    std::string const pred_str = oss.str();
    if (!pred_str.empty()) {
        res += " where " + pred_str;
    }
    os << res;
    return os;
}

QueryData::QueryData(std::vector<std::string> fields, std::vector<std::string> tables, Predicate pred)
    : fields_m(std::move(fields)), tables_m(std::move(tables)), pred_m(std::move(pred)) {}

auto QueryData::fields() -> std::vector<std::string> {
    return fields_m;
}

auto QueryData::tables() -> std::vector<std::string> {
    return tables_m;
}

auto QueryData::pred() -> Predicate {
    return pred_m;
}
