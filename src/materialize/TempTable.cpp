//
// Created by G on 11/23/2024.
//

#include "TempTable.h"
int TempTable::next_table_num = 0;

TempTable::TempTable(Transaction *tx, Schema sch) : tx(tx), layout(sch) {
    tbl_name = nextTableName();
}

auto TempTable::open() -> TableScan {
    return {tx, tbl_name, layout};
}

auto TempTable::tableName() -> std::string {
    return tbl_name;
}

auto TempTable::getLayout() -> Layout {
    return layout;
}

auto TempTable::nextTableName() -> std::string {
    std::unique_lock<std::mutex> const lock(mutex);
    next_table_num++;
    return "temp" + std::to_string(next_table_num);
}
