//
// Created by G on 11/23/2024.
//

#include "GroupValue.h"

auto GroupValue::operator==(const GroupValue& rhs) const -> bool {
    return vals == rhs.vals;
}

auto GroupValue::operator!=(const GroupValue& rhs) const -> bool {
    return !(rhs == *this);
}

GroupValue::GroupValue(Scan* s, std::vector<std::string>& fields) {
    for (auto& fld_name : fields) {
        vals[fld_name] = std::make_unique<Constant>(s->getVal(fld_name));
    }
}

auto GroupValue::getVal(std::string& fld_name) -> Constant {
    return *vals[fld_name];
}

auto GroupValue::hashCode() -> int {
    int64_t res = 0;
    for (auto& entry : vals) {
        res += entry.second->hashCode();
    }
    return static_cast<int>(res);
}
