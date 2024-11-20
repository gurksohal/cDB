//
// Created by G on 11/19/2024.
//

#include "ProjectScan.h"

#include <utility>

void ProjectScan::beforeFirst() {
    s->beforeFirst();
}

auto ProjectScan::next() -> bool {
    return s->next();
}

auto ProjectScan::getInt(const std::string& fld_name) -> int {
    return s->getInt(fld_name);
}

auto ProjectScan::getString(const std::string& fld_name) -> std::string {
    return s->getString(fld_name);
}

auto ProjectScan::getVal(const std::string& fld_name) -> Constant {
    return s->getVal(fld_name);
}

auto ProjectScan::hasField(const std::string& fld_name) -> bool {
    return s->hasField(fld_name);
}

void ProjectScan::close() {
    s->close();
}

ProjectScan::ProjectScan(std::shared_ptr<Scan> s, std::vector<std::string>& fld_list)
    : s(std::move(s)), fld_list(fld_list) {}
