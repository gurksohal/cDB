//
// Created by G on 11/15/2024.
//

#include "TableMgr.h"

#include "../record/TableScan.h"

TableMgr::TableMgr(bool is_new, Transaction& tx) {
    Schema tcat_schema;
    tcat_schema.addStringField("tblname", MAX_NAME);
    tcat_schema.addIntField("slotsize");
    tcat_layout = Layout(tcat_schema);

    Schema fcat_schema;
    fcat_schema.addStringField("tblname", MAX_NAME);
    fcat_schema.addStringField("fldname", MAX_NAME);
    fcat_schema.addIntField("type");
    fcat_schema.addIntField("length");
    fcat_schema.addIntField("offset");
    fcat_layout = Layout(fcat_schema);

    if (is_new) {
        createTable("tblcat", tcat_schema, tx);
        createTable("fldcat", fcat_schema, tx);
    }
}

auto TableMgr::createTable(const std::string& tbl_name, Schema& schema, Transaction& tx) -> void {
    Layout layout(schema);
    TableScan tcat(&tx, "tblcat", tcat_layout);
    tcat.insert();
    tcat.setString("tblname", tbl_name);
    tcat.setInt("slotsize", layout.slotSize());
    tcat.close();

    TableScan fcat(&tx, "fldcat", fcat_layout);
    for (const auto& fld_name : schema.fields()) {
        fcat.insert();
        fcat.setString("tblname", tbl_name);
        fcat.setString("fldname", fld_name);
        fcat.setInt("type", schema.type(fld_name));
        fcat.setInt("length", schema.length(fld_name));
        fcat.setInt("offset", layout.offset(fld_name));
    }
    fcat.close();
}

auto TableMgr::getLayout(const std::string& tbl_name, Transaction& tx) -> Layout {
    int size = -1;
    TableScan tcat(&tx, "tblcat", tcat_layout);
    while (tcat.next()) {
        if (tcat.getString("tblname") == tbl_name) {
            size = tcat.getInt("slotsize");
            break;
        }
    }
    tcat.close();

    Schema sch;
    std::unordered_map<std::string, int> offsets;
    TableScan fcat(&tx, "fldcat", fcat_layout);
    while (fcat.next()) {
        if (fcat.getString("tblname") == tbl_name) {
            auto fld_name = fcat.getString("fldname");
            int const fld_type = fcat.getInt("type");
            int const fld_len = fcat.getInt("length");
            int const offset = fcat.getInt("offset");
            offsets.insert({fld_name, offset});
            sch.addField(fld_name, fld_type, fld_len);
        }
    }
    fcat.close();
    return Layout(sch, offsets, size);
}
