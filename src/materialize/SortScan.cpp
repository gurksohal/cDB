//
// Created by G on 11/24/2024.
//

#include "SortScan.h"

SortScan::SortScan(std::vector<std::unique_ptr<TempTable>>& runs, RecordComparator& comp)
    : comp(comp), has_more1(false), has_more2(false) {
    s1 = std::make_shared<TableScan>(runs[0]->open());
    auto t = s1->next();
    has_more1 = t;
    if (runs.size() > 1) {
        s2 = std::make_shared<TableScan>(runs[1]->open());
        has_more2 = s2->next();
    }
}

void SortScan::beforeFirst() {
    s1->beforeFirst();
    has_more1 = s1->next();
    if (s2) {
        s2->beforeFirst();
        has_more2 = s2->next();
    }
}

auto SortScan::next() -> bool {
    if (curr_scan) {
        if (curr_scan.get() == s1.get()) {
            has_more1 = s2->next();
        } else if (curr_scan.get() == s2.get()) {
            has_more2 = s2->next();
        }
    }

    if (!has_more1 && !has_more2) {
        return false;
    }

    if (has_more1 && !has_more2) {
        if (comp.compare(s1.get(), s2.get()) < 0) {
            curr_scan = s1;
        } else {
            curr_scan = s2;
        }
    } else if (has_more1) {
        curr_scan = s1;
    } else {
        curr_scan = s2;
    }

    return true;
}

auto SortScan::getInt(const std::string& fld_name) -> int {
    return curr_scan->getInt(fld_name);
}

auto SortScan::getString(const std::string& fld_name) -> std::string {
    return curr_scan->getString(fld_name);
}

auto SortScan::getVal(const std::string& fld_name) -> Constant {
    return curr_scan->getVal(fld_name);
}

auto SortScan::hasField(const std::string& fld_name) -> bool {
    return curr_scan->hasField(fld_name);
}

void SortScan::close() {
    s1->close();
    if (s2) {
        s2->close();
    }
}

auto SortScan::savePosition() -> void {
    auto rid1 = s1->getRid();
    RID rid2 {-1, -1};
    if (s2) {
        rid2 = s2->getRid();
    }

    saved_position = {rid1, rid2};
}

auto SortScan::restorePosition() -> void {
    auto rid1 = saved_position[0];
    auto rid2 = saved_position[1];
    s1->moveToRID(rid1);
    if (rid2.slot() != -1 && rid2.blockNumber() != -1) {
        s2->moveToRID(rid2);
    }
}
