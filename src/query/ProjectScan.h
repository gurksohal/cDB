//
// Created by G on 11/19/2024.
//

#ifndef CDB_PROJECTSCAN_H
#define CDB_PROJECTSCAN_H

#include <memory>
#include <vector>

#include "Scan.h"

class ProjectScan : public Scan {
public:
    ProjectScan(std::shared_ptr<Scan> s, std::vector<std::string>& fld_list);
    auto beforeFirst() -> void override;
    auto next() -> bool override;
    auto getInt(const std::string& fld_name) -> int override;
    auto getString(const std::string& fld_name) -> std::string override;
    auto getVal(const std::string& fld_name) -> Constant override;
    auto hasField(const std::string& fld_name) -> bool override;
    auto close() -> void override;

private:
    std::shared_ptr<Scan> s;
    std::vector<std::string> fld_list;
};

#endif  // CDB_PROJECTSCAN_H
