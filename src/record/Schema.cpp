//
// Created by G on 11/4/2024.
//

#include "Schema.h"

auto Schema::addField(const std::string& fld_name, int type, int length) -> void {
    all_fields.push_back(fld_name);
    info.emplace(fld_name, FieldInfo(type, length));
}

auto Schema::addIntField(const std::string& fld_name) -> void {
    addField(fld_name, INTEGER, 0);
}

auto Schema::addStringField(const std::string& fld_name, int length) -> void {
    addField(fld_name, VARCHAR, length);
}

auto Schema::add(const std::string& fld_name, const Schema& sch) -> void {
    auto type = sch.type(fld_name);
    auto length = sch.length(fld_name);
    addField(fld_name, type, length);
}

auto Schema::addAll(const Schema& sch) -> void {
    for (const auto& name : sch.fields()) {
        add(name, sch);
    }
}

auto Schema::fields() const -> const std::vector<std::string>& {
    return all_fields;
}

auto Schema::hasField(const std::string& fld_name) const -> bool {
    return std::find(all_fields.begin(), all_fields.end(), fld_name) != all_fields.end();
}

auto Schema::type(const std::string& fld_name) const -> int {
    return info.at(fld_name).type;
}

auto Schema::length(const std::string& fld_name) const -> int {
    return info.at(fld_name).length;
}
