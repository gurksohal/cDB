//
// Created by G on 11/4/2024.
//

#ifndef CDB_SCHEMA_H
#define CDB_SCHEMA_H

#include <string>
#include <unordered_map>
#include <vector>

class Schema {
public:
    enum Type : std::uint8_t { INTEGER = 0, VARCHAR = 1 };

    struct FieldInfo {
        int type;
        int length;

        FieldInfo(int type, int length) : type(type), length(length) {}
    };

    auto addField(const std::string& fld_name, int type, int length) -> void;
    auto addIntField(const std::string& fld_name) -> void;
    auto addStringField(const std::string& fld_name, int length) -> void;
    auto add(const std::string& fld_name, const Schema& sch) -> void;
    auto addAll(Schema& sch) -> void;
    auto fields() -> std::vector<std::string>&;
    auto hasField(const std::string& fld_name) const -> bool;
    auto type(const std::string& fld_name) const -> int;
    auto length(const std::string& fld_name) const -> int;

private:
    std::vector<std::string> all_fields;
    std::unordered_map<std::string, FieldInfo> info;
};

#endif  // CDB_SCHEMA_H
