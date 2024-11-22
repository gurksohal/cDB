//
// Created by G on 11/4/2024.
//

#ifndef CDB_LAYOUT_H
#define CDB_LAYOUT_H

#include "Schema.h"

class Layout {
public:
    Layout() = default;
    explicit Layout(Schema& schema);
    explicit Layout(Schema schema, const std::unordered_map<std::string, int>& offsets, int slot_size);
    auto schema() -> Schema;
    auto offset(const std::string& fld_name) -> int;
    auto slotSize() const -> int;

private:
    auto lengthInBytes(const std::string& fld_name) -> int;
    Schema schema_m;
    std::unordered_map<std::string, int> offsets;
    int slot_size {};
};

#endif  // CDB_LAYOUT_H
