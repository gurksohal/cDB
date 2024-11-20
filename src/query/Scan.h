//
// Created by G on 11/19/2024.
//

#ifndef CDB_SCAN_H
#define CDB_SCAN_H

#include <string>

#include "Constant.h"

class Scan {
public:
    Scan() = default;
    Scan(Scan&&) = default;
    Scan(const Scan&) = default;
    auto operator=(const Scan&) -> Scan& = default;
    auto operator=(Scan&&) -> Scan& = default;
    virtual ~Scan() = default;
    virtual auto beforeFirst() -> void = 0;
    virtual auto next() -> bool = 0;
    virtual auto getInt(const std::string& fld_name) -> int = 0;
    virtual auto getString(const std::string& fld_name) -> std::string = 0;
    virtual auto getVal(const std::string& fld_name) -> Constant = 0;
    virtual auto hasField(const std::string& fld_name) -> bool = 0;
    virtual auto close() -> void = 0;
};

#endif  // CDB_SCAN_H
