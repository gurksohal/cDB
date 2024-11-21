//
// Created by G on 11/20/2024.
//

#ifndef CDB_DELETADATA_H
#define CDB_DELETADATA_H
#include "../query/Predicate.h"

class DeleteData {
public:
    DeleteData(std::string& tbl_name, Predicate& pred) : tbl_name(tbl_name), pred_m(pred) {}

    auto tableName() -> std::string {
        return tbl_name;
    }

    auto pred() -> Predicate {
        return pred_m;
    }

private:
    std::string tbl_name;
    Predicate pred_m;
};
#endif  // CDB_DELETADATA_H
