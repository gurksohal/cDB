//
// Created by G on 11/24/2024.
//

#include "MergeJoinScan.h"

MergeJoinScan::MergeJoinScan(std::shared_ptr<Scan>& s1, std::shared_ptr<SortScan>& s2, std::string& fld_name1,
                             std::string& fld_name2)
    : s1(s1), s2(s2), fld_name1(fld_name1), fld_name2(fld_name2), join_val(INT_MIN) {
    s1->beforeFirst();
    s2->beforeFirst();
}

void MergeJoinScan::beforeFirst() {
    s1->beforeFirst();
    s2->beforeFirst();
}

auto MergeJoinScan::next() -> bool {
    bool has_more2 = s2->next();
    if (has_more2 && s2->getVal(fld_name2) == join_val) {
        return true;
    }

    bool has_more1 = s1->next();
    if (has_more1 && s1->getVal(fld_name1) == join_val) {
        s2->restorePosition();
        return true;
    }

    while (has_more1 && has_more2) {
        auto v1 = s1->getVal(fld_name1);
        auto v2 = s2->getVal(fld_name2);
        if (v1 < v2) {
            has_more1 = s1->next();
        } else if (v1 > v2) {
            has_more2 = s2->next();
        } else {
            s2->savePosition();
            join_val = s2->getVal(fld_name2);
            return true;
        }
    }
    return false;
}

auto MergeJoinScan::getInt(const std::string& fld_name) -> int {
    if (s1->hasField(fld_name)) {
        return s1->getInt(fld_name);
    }
    return s2->getInt(fld_name);
}

auto MergeJoinScan::getString(const std::string& fld_name) -> std::string {
    if (s1->hasField(fld_name)) {
        return s1->getString(fld_name);
    }
    return s2->getString(fld_name);
}

auto MergeJoinScan::getVal(const std::string& fld_name) -> Constant {
    if (s1->hasField(fld_name)) {
        return s1->getVal(fld_name);
    }
    return s2->getVal(fld_name);
}

auto MergeJoinScan::hasField(const std::string& fld_name) -> bool {
    return s1->hasField(fld_name) || s2->hasField(fld_name);
}

void MergeJoinScan::close() {
    s1->close();
    s2->close();
}
