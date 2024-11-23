//
// Created by G on 11/24/2024.
//

#ifndef CDB_RECORDCOMPARATOR_H
#define CDB_RECORDCOMPARATOR_H
#include <string>
#include <vector>

#include "../query/Scan.h"

class RecordComparator {
public:
    explicit RecordComparator(std::vector<std::string>& fields) : fields(fields) {}

    auto compare(Scan* s1, Scan* s2) -> int {
        for (auto& f : fields) {
            auto v1 = s1->getVal(f);
            auto v2 = s2->getVal(f);
            if (v1 > v2) {
                return 1;
            }
            if (v2 > v1) {
                return -1;
            }
        }

        return 0;
    }

private:
    std::vector<std::string> fields;
};
#endif  // CDB_RECORDCOMPARATOR_H
