//
// Created by G on 11/19/2024.
//

#ifndef CDB_CONSTANT_H
#define CDB_CONSTANT_H

#include <ostream>
#include <string>

class Constant {
public:
    explicit Constant(int i_val);
    explicit Constant(std::string s_val);
    auto asInt() const -> int;
    auto asString() -> std::string;
    auto operator==(const Constant& rhs) const -> bool;
    auto operator!=(const Constant& rhs) const -> bool;
    friend auto operator<<(std::ostream& os, const Constant& constant) -> std::ostream&;

private:
    int i_val;
    std::string s_val;
};

#endif  // CDB_CONSTANT_H
