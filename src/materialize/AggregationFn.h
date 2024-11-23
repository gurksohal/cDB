//
// Created by G on 11/23/2024.
//

#ifndef CDB_AGGREGATIONFN_H
#define CDB_AGGREGATIONFN_H
#include <memory>

#include "../query/Scan.h"

class AggregationFn {
public:
    AggregationFn() = default;
    AggregationFn(AggregationFn&) = delete;
    AggregationFn(AggregationFn&&) = delete;
    auto operator=(AggregationFn&) = delete;
    auto operator=(AggregationFn&&) = delete;
    virtual ~AggregationFn() = default;
    virtual auto processFirst(std::shared_ptr<Scan> s) -> void = 0;
    virtual auto processNext(std::shared_ptr<Scan> s) -> void = 0;
    virtual auto fieldName() -> std::string = 0;
    virtual auto value() -> Constant = 0;
};
#endif  // CDB_AGGREGATIONFN_H
