//
// Created by G on 11/15/2024.
//

#ifndef CDB_STATINFO_H
#define CDB_STATINFO_H

class StatInfo {
public:
    StatInfo() = default;
    StatInfo(int blocks, int recs);
    auto blockAccessed() const -> int;
    auto recordsOutput() const -> int;
    auto distinctValues() const -> int;

private:
    int num_blocks;
    int num_recs;
};

#endif  // CDB_STATINFO_H
