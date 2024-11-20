//
// Created by G on 11/19/2024.
//

#ifndef CDB_UPDATESCAN_H
#define CDB_UPDATESCAN_H

#include "../record/RID.h"
#include "Scan.h"

class UpdateScan : public Scan {
public:
    virtual auto setVal(const std::string& fld_name, Constant val) -> void = 0;
    virtual auto setInt(const std::string& file_name, int val) -> void = 0;
    virtual auto setString(const std::string& file_name, const std::string& val) -> void = 0;
    virtual auto insert() -> void = 0;
    virtual auto remove() -> void = 0;
    virtual auto getRid() -> RID = 0;
    virtual auto moveToRID(const RID& rid) -> void = 0;
};

#endif  // CDB_UPDATESCAN_H
