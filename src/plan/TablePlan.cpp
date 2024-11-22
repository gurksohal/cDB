//
// Created by G on 11/21/2024.
//

#include "TablePlan.h"

#include "../record/TableScan.h"

TablePlan::TablePlan(Transaction* tx, const std::string& tbl_name, MetadataMgr* md)
    : tbl_name(tbl_name), tx(tx), layout(md->getLayout(tbl_name, *tx)), si(md->getStatInfo(tbl_name, layout, *tx)) {}

auto TablePlan::open() -> std::shared_ptr<Scan> {
    std::shared_ptr<Scan> ret = std::make_shared<TableScan>(tx, tbl_name, layout);
    return ret;
}

auto TablePlan::blocksAccessed() -> int {
    return si.blockAccessed();
}

auto TablePlan::recordsOutput() -> int {
    return si.recordsOutput();
}

auto TablePlan::distinctValues() -> int {
    return si.distinctValues();
}

auto TablePlan::schema() -> Schema {
    return layout.schema();
}
