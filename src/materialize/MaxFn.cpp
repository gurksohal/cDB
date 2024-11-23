//
// Created by G on 11/23/2024.
//

#include "MaxFn.h"

MaxFn::MaxFn(std::string fld_name) : fld_name(std::move(fld_name)), val(INT_MIN) {}

void MaxFn::processFirst(std::shared_ptr<Scan> s) {
    val = s->getVal(fld_name);
}

void MaxFn::processNext(std::shared_ptr<Scan> s) {
    auto new_val = s->getVal(fld_name);
    if (new_val > val) {
        val = new_val;
    }
}

auto MaxFn::fieldName() -> std::string {
    return "maxof" + fld_name;
}

auto MaxFn::value() -> Constant {
    return val;
}
