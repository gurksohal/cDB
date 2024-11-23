//
// Created by G on 11/23/2024.
//

#ifndef CDB_MAXFN_H
#define CDB_MAXFN_H

#include "AggregationFn.h"

class MaxFn : public AggregationFn {
public:
    explicit MaxFn(std::string fld_name);
    auto processFirst(std::shared_ptr<Scan> s) -> void override;
    auto processNext(std::shared_ptr<Scan> s) -> void override;
    auto fieldName() -> std::string override;
    auto value() -> Constant override;

private:
    std::string fld_name;
    Constant val;
};

#endif  // CDB_MAXFN_H
