//
// Created by G on 11/15/2024.
//

#ifndef CDB_VIEWMGR_H
#define CDB_VIEWMGR_H

#include "TableMgr.h"

class ViewMgr {
public:
    ViewMgr(bool is_new, TableMgr* tm, Transaction& tx);
    auto createView(const std::string& v_name, const std::string& v_def, Transaction& tx) -> void;
    auto getViewDef(const std::string& v_name, Transaction& tx) -> std::string;

private:
    const static int MAX_VIEW_DEF = 100;
    TableMgr* tbl_mgr;
};

#endif  // CDB_VIEWMGR_H
