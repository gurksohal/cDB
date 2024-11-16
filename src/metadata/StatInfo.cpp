//
// Created by G on 11/15/2024.
//

#include "StatInfo.h"

StatInfo::StatInfo(int blocks, int recs) : num_blocks(blocks), num_recs(recs) {}

auto StatInfo::blockAccessed() const -> int {
    return num_blocks;
}

auto StatInfo::recordsOutput() const -> int {
    return num_recs;
}

auto StatInfo::distinctValues() const -> int {
    return 1 + (num_recs / 3);
}
