//
// Created by G on 11/15/2024.
//

#include "IndexInfo.h"

#include <utility>

IndexInfo::IndexInfo(std::string name, std::string fld, Schema tbl_schema, Transaction *tx, StatInfo si)
    : idx_name(std::move(name)),
      fld_name(std::move(fld)),
      tx(tx),
      tbl_schema(std::move(tbl_schema)),
      idx_layout(createIdxLayout()),
      si(si) {}

auto IndexInfo::open() -> void {
    auto s = fld_name;
    // TODO: after index
    throw std::runtime_error("to implement after index: " + s);
}

auto IndexInfo::blocksAccessed() -> int {
    int const rpb = tx->blockSize() / idx_layout.slotSize();
    int const num_blocks = si.recordsOutput() / rpb;
    // TODO: after index
    throw std::runtime_error("to implement after index" + std::to_string(num_blocks));
}

auto IndexInfo::recordsOutput() -> int {
    return si.recordsOutput() / si.distinctValues();
}

auto IndexInfo::distinctValues(const std::string &fld) -> int {
    return fld_name == fld ? 1 : si.distinctValues();
}

auto IndexInfo::createIdxLayout() -> Layout {
    Schema sch;
    sch.addIntField("block");
    sch.addIntField("id");
    if (tbl_schema.type(fld_name) == Schema::INTEGER) {
        sch.addIntField("dataval");
    } else {
        int const fld_len = tbl_schema.length(fld_name);
        sch.addStringField("dataval", fld_len);
    }

    return Layout {sch};
}
