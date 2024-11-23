//
// Created by G on 11/23/2024.
//

#include "CountFn.h"

#include <utility>

CountFn::CountFn(std::string fld_name) : fld_name(std::move(fld_name)), count(0) {}

void CountFn::processFirst(std::shared_ptr<Scan> /*s*/) {
    count = 1;
}

void CountFn::processNext(std::shared_ptr<Scan> /*s*/) {
    count++;
}

auto CountFn::fieldName() -> std::string {
    return "countof" + fld_name;
}

auto CountFn::value() -> Constant {
    return Constant(count);
}
