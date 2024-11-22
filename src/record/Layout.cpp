//
// Created by G on 11/4/2024.
//

#include "Layout.h"

#include <utility>

#include "../file/Page.h"

Layout::Layout(Schema& schema) : schema_m(schema) {
    int pos = static_cast<int>(sizeof(int));
    for (auto& name : schema.fields()) {
        offsets[name] = pos;
        pos += lengthInBytes(name);
    }
    slot_size = pos;
}

Layout::Layout(Schema schema, const std::unordered_map<std::string, int>& offsets, int slot_size)
    : schema_m(std::move(schema)), offsets(offsets), slot_size(slot_size) {}

auto Layout::schema() -> Schema {
    return schema_m;
}

auto Layout::offset(const std::string& fld_name) -> int {
    return offsets[fld_name];
}

auto Layout::slotSize() const -> int {
    return slot_size;
}

auto Layout::lengthInBytes(const std::string& fld_name) -> int {
    int const fld_type = schema_m.type(fld_name);
    if (fld_type == Schema::Type::INTEGER) {
        return sizeof(int);
    }
    return Page::maxLength(schema_m.length(fld_name));
}
