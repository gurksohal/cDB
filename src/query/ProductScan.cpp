//
// Created by G on 11/19/2024.
//

#include "ProductScan.h"

void ProductScan::beforeFirst() {
    s1->beforeFirst();
    s1->next();
    s2->beforeFirst();
}

auto ProductScan::next() -> bool {
    if (s2->next()) {
        return true;
    }
    s2->beforeFirst();
    return s2->next() && s1->next();
}

auto ProductScan::getInt(const std::string& fld_name) -> int {
    if (s1->hasField(fld_name)) {
        return s1->getInt(fld_name);
    }

    return s2->getInt(fld_name);
}

auto ProductScan::getString(const std::string& fld_name) -> std::string {
    if (s1->hasField(fld_name)) {
        return s1->getString(fld_name);
    }

    return s2->getString(fld_name);
}

auto ProductScan::getVal(const std::string& fld_name) -> Constant {
    if (s1->hasField(fld_name)) {
        return s1->getVal(fld_name);
    }

    return s2->getVal(fld_name);
}

auto ProductScan::hasField(const std::string& fld_name) -> bool {
    return s1->hasField(fld_name) || s2->hasField(fld_name);
}

void ProductScan::close() {
    s1->close();
    s2->close();
}

ProductScan::ProductScan(std::shared_ptr<Scan>& s1, std::shared_ptr<Scan>& s2) : s1(s1), s2(s2) {
    s1->beforeFirst();
    s1->next();
    s2->beforeFirst();
}
