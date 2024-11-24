//
// Created by G on 11/24/2024.
//

#include "BufferNeeds.h"

#include <climits>
#include <cmath>

auto BufferNeeds::bestRoot(int avaiable, int size) -> int {
    int const avail = avaiable - 2;
    if (avail <= 1) {
        return 1;
    }

    int k = INT_MAX;
    double i = 1.0;
    while (k > avail) {
        i++;
        k = static_cast<int>(std::ceil(std::pow(size, 1 / i)));
    }

    return k;
}

auto BufferNeeds::bestFactor(int available, int size) -> int {
    int const avail = available - 2;  // reserve a couple
    if (avail <= 1) {
        return 1;
    }
    int k = size;
    double i = 1.0;
    while (k > avail) {
        i++;
        k = static_cast<int>(std::ceil(size / i));
    }
    return k;
}
