//
// Created by G on 11/19/2024.
//

#include "SelectScan.h"

SelectScan::SelectScan(std::shared_ptr<Scan> s, Predicate& p) : scan(std::move(s)), pred(p) {}

void SelectScan::beforeFirst() {
    scan->beforeFirst();
}

auto SelectScan::next() -> bool {
    while (scan->next()) {
        if (pred.isSatisfied(*scan)) {
            return true;
        }
    }
    return false;
}

auto SelectScan::getInt(const std::string& fld_name) -> int {
    return scan->getInt(fld_name);
}

auto SelectScan::getString(const std::string& fld_name) -> std::string {
    return scan->getString(fld_name);
}

auto SelectScan::getVal(const std::string& fld_name) -> Constant {
    return scan->getVal(fld_name);
}

auto SelectScan::hasField(const std::string& fld_name) -> bool {
    return scan->hasField(fld_name);
}

void SelectScan::close() {
    scan->close();
}

void SelectScan::setVal(const std::string& fld_name, Constant val) {
    auto us = std::dynamic_pointer_cast<UpdateScan>(scan);
    us->setVal(fld_name, val);
}

void SelectScan::setInt(const std::string& fld_name, int val) {
    auto us = std::dynamic_pointer_cast<UpdateScan>(scan);
    us->setInt(fld_name, val);
}

void SelectScan::setString(const std::string& fld_name, const std::string& val) {
    auto us = std::dynamic_pointer_cast<UpdateScan>(scan);
    us->setString(fld_name, val);
}

void SelectScan::insert() {
    auto us = std::dynamic_pointer_cast<UpdateScan>(scan);
    us->insert();
}

void SelectScan::remove() {
    auto us = std::dynamic_pointer_cast<UpdateScan>(scan);
    us->remove();
}

auto SelectScan::getRid() -> RID {
    auto us = std::dynamic_pointer_cast<UpdateScan>(scan);
    return us->getRid();
}

auto SelectScan::moveToRID(const RID& rid) -> void {
    auto us = std::dynamic_pointer_cast<UpdateScan>(scan);
    us->moveToRID(rid);
}
