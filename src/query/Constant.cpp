//
// Created by G on 11/19/2024.
//

#include "Constant.h"

#include <utility>

auto Constant::operator==(const Constant& rhs) const -> bool {
    return i_val == rhs.i_val && s_val == rhs.s_val;
}

auto Constant::operator!=(const Constant& rhs) const -> bool {
    return !(rhs == *this);
}

auto operator<<(std::ostream& os, const Constant& constant) -> std::ostream& {
    if (constant.s_val.empty()) {
        os << std::to_string(constant.i_val);
    } else {
        os << constant.s_val;
    }
    return os;
}

Constant::Constant(int i_val) : i_val(i_val) {}

Constant::Constant(std::string s_val) : s_val(std::move(s_val)), i_val(-1) {}

auto Constant::asInt() const -> int {
    return i_val;
}

auto Constant::asString() -> std::string {
    return s_val;
}

auto Constant::hashCode() -> int {
    if (s_val.empty()) {
        return static_cast<int>(std::hash<int> {}(i_val));
    }

    return static_cast<int>(std::hash<std::string> {}(s_val));
}

auto Constant::operator<(const Constant& rhs) const -> bool {
    if (s_val < rhs.s_val) {
        return true;
    }
    if (rhs.s_val < s_val) {
        return false;
    }
    return i_val < rhs.i_val;
}

auto Constant::operator>(const Constant& rhs) const -> bool {
    return rhs < *this;
}

auto Constant::operator<=(const Constant& rhs) const -> bool {
    return !(rhs < *this);
}

auto Constant::operator>=(const Constant& rhs) const -> bool {
    return !(*this < rhs);
}
