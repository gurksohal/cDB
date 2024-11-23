//
// Created by G on 11/23/2024.
//

#ifndef CDB_GROUPVALUE_H
#define CDB_GROUPVALUE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../query/Constant.h"
#include "../query/Scan.h"

class GroupValue {
public:
    GroupValue(Scan* s, std::vector<std::string>& fields);
    auto getVal(std::string& fld_name) -> Constant;
    auto operator==(const GroupValue& rhs) const -> bool;
    auto operator!=(const GroupValue& rhs) const -> bool;
    auto hashCode() -> int;

private:
    std::unordered_map<std::string, std::unique_ptr<Constant>> vals;
};

#endif  // CDB_GROUPVALUE_H
