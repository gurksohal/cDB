#include <iostream>
#include <vector>

#include "server/CDB.h"

namespace {
auto trim(std::string& s) -> void {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

auto start() -> void {
    auto dir = std::filesystem::current_path() / "studentdb";
    CDB db(dir);
    std::cout << "> ";
    std::string query;
    while (std::getline(std::cin, query)) {
        trim(query);
        if (query == "exit") {
            break;
        }

        try {
            if (query.starts_with("select")) {
                db.execQuery(query);
            } else {
                int const r = db.execUpdate(query);
                std::cout << "changed records: " << r << "\n";
            }
        } catch (std::exception e) {  // NOLINT
            std::cout << "error: " << e.what() << "\n";
        }
        std::cout << "> ";
    }
}
}  // namespace

auto main() -> int {
    start();
    return 0;
}
