//
// Created by G on 11/20/2024.
//

#ifndef CDB_MODIFYDATA_H
#define CDB_MODIFYDATA_H
#include <string>
#include <utility>

#include "../query/Expression.h"
#include "../query/Predicate.h"

class ModifyData {
public:
    ModifyData(std::string tbl_name, std::string fld_name, Expression new_val, Predicate pred)
        : tbl_name(std::move(tbl_name)),
          fld_name(std::move(fld_name)),
          new_val(std::move(new_val)),
          pred_m(std::move(pred)) {}

    auto tableName() -> std::string {
        return tbl_name;
    }

    auto targetField() -> std::string {
        return fld_name;
    }

    auto newValue() -> Expression {
        return new_val;
    }

    auto pred() -> Predicate {
        return pred_m;
    }

private:
    std::string tbl_name;
    std::string fld_name;
    Expression new_val;
    Predicate pred_m;
};
#endif  // CDB_MODIFYDATA_H
