//
// Created by G on 11/23/2024.
//

#ifndef CDB_COUNTFN_H
#define CDB_COUNTFN_H

#include "AggregationFn.h"

class CountFn : public AggregationFn {
public:
    explicit CountFn(std::string fld_name);
    auto processFirst(std::shared_ptr<Scan> s) -> void override;
    auto processNext(std::shared_ptr<Scan> s) -> void override;
    auto fieldName() -> std::string override;
    auto value() -> Constant override;

private:
    std::string fld_name;
    int count;
};

#endif  // CDB_COUNTFN_H
