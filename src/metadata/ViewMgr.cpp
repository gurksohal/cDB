//
// Created by G on 11/15/2024.
//

#include "ViewMgr.h"

#include "../record/TableScan.h"

ViewMgr::ViewMgr(bool is_new, TableMgr *tm, Transaction &tx) : tbl_mgr(tm) {
    if (is_new) {
        Schema sch;
        sch.addStringField("viewname", TableMgr::MAX_NAME);
        sch.addStringField("viewdef", MAX_VIEW_DEF);
        tbl_mgr->createTable("viewcat", sch, tx);
    }
}

auto ViewMgr::createView(const std::string &v_name, const std::string &v_def, Transaction &tx) -> void {
    Layout const layout = tbl_mgr->getLayout("viewcat", tx);
    TableScan ts(&tx, "viewcat", layout);
    ts.insert();
    ts.setString("viewname", v_name);
    ts.setString("viewdef", v_def);
    ts.close();
}

auto ViewMgr::getViewDef(const std::string &v_name, Transaction &tx) -> std::string {
    std::string res;
    Layout const layout = tbl_mgr->getLayout("viewcat", tx);
    TableScan ts(&tx, "viewcat", layout);
    while (ts.next()) {
        if (ts.getString("viewname") == v_name) {
            res = ts.getString("viewdef");
            break;
        }
    }

    return res;
}
