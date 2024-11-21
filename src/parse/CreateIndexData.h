//
// Created by G on 11/20/2024.
//

#ifndef CDB_CREATEINDEXDATA_H
#define CDB_CREATEINDEXDATA_H

#include <string>
#include <utility>

class CreateIndexData {
public:
    CreateIndexData(std::string idx, std::string tbl, std::string fld);
    auto indexName() -> std::string;
    auto tableName() -> std::string;
    auto fieldName() -> std::string;

private:
    std::string idx_name;
    std::string tbl_name;
    std::string fld_name;
};

inline CreateIndexData::CreateIndexData(std::string idx, std::string tbl, std::string fld)
    : idx_name(std::move(idx)), tbl_name(std::move(tbl)), fld_name(std::move(fld)) {}

inline auto CreateIndexData::indexName() -> std::string {
    return idx_name;
}

inline auto CreateIndexData::tableName() -> std::string {
    return tbl_name;
}

inline auto CreateIndexData::fieldName() -> std::string {
    return fld_name;
}

#endif  // CDB_CREATEINDEXDATA_H
