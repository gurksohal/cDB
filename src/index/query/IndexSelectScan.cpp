//
// Created by G on 11/23/2024.
//

#include "IndexSelectScan.h"

#include <utility>

IndexSelectScan::IndexSelectScan(std::shared_ptr<TableScan> ts, std::shared_ptr<Index> idx,
                                 std::shared_ptr<Constant> val)
    : ts(std::move(ts)), idx(std::move(idx)), val(std::move(val)) {
    this->idx->beforeFirst(this->val);
}

void IndexSelectScan::beforeFirst() {
    idx->beforeFirst(val);
}

auto IndexSelectScan::next() -> bool {
    bool const ok = idx->next();
    if (ok) {
        RID const rid = idx->getDataRid();
        ts->moveToRID(rid);
    }
    return ok;
}

auto IndexSelectScan::getInt(const std::string& fld_name) -> int {
    return ts->getInt(fld_name);
}

auto IndexSelectScan::getString(const std::string& fld_name) -> std::string {
    return ts->getString(fld_name);
}

auto IndexSelectScan::getVal(const std::string& fld_name) -> Constant {
    return ts->getVal(fld_name);
}

auto IndexSelectScan::hasField(const std::string& fld_name) -> bool {
    return ts->hasField(fld_name);
}

void IndexSelectScan::close() {
    idx->close();
    ts->close();
}
