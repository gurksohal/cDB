//
// Created by G on 11/23/2024.
//

#include "IndexJoinScan.h"

#include <utility>

IndexJoinScan::IndexJoinScan(std::shared_ptr<Scan> lhs, std::shared_ptr<Index> idx, std::string join_field,
                             std::shared_ptr<TableScan> rhs)
    : lhs(std::move(lhs)), idx(std::move(idx)), rhs(std::move(rhs)), join_field(std::move(join_field)) {
    this->lhs->beforeFirst();
    this->lhs->next();
    resetIndex();
}

void IndexJoinScan::beforeFirst() {
    lhs->beforeFirst();
    lhs->next();
    resetIndex();
}

auto IndexJoinScan::next() -> bool {
    while (true) {
        if (idx->next()) {
            rhs->moveToRID(idx->getDataRid());
            return true;
        }
        if (!lhs->next()) {
            return false;
        }
        resetIndex();
    }
}

auto IndexJoinScan::getInt(const std::string& fld_name) -> int {
    if (rhs->hasField(fld_name)) {
        return rhs->getInt(fld_name);
    }
    return lhs->getInt(fld_name);
}

auto IndexJoinScan::getString(const std::string& fld_name) -> std::string {
    if (rhs->hasField(fld_name)) {
        return rhs->getString(fld_name);
    }
    return lhs->getString(fld_name);
}

auto IndexJoinScan::getVal(const std::string& fld_name) -> Constant {
    if (rhs->hasField(fld_name)) {
        return rhs->getVal(fld_name);
    }
    return lhs->getVal(fld_name);
}

auto IndexJoinScan::hasField(const std::string& fld_name) -> bool {
    return rhs->hasField(fld_name) || lhs->hasField(fld_name);
}

void IndexJoinScan::close() {
    lhs->close();
    rhs->close();
    idx->close();
}

auto IndexJoinScan::resetIndex() -> void {
    auto search_key = lhs->getVal(join_field);
    idx->beforeFirst(std::make_shared<Constant>(search_key));
}
