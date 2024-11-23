//
// Created by G on 11/23/2024.
//

#ifndef CDB_GROUPBYSCAN_H
#define CDB_GROUPBYSCAN_H

#include <memory>
#include <vector>

#include "../query/Scan.h"
#include "AggregationFn.h"
#include "GroupValue.h"

class GroupByScan : public Scan {
public:
    GroupByScan(std::shared_ptr<Scan> s, std::vector<std::string> group_fields,
                std::vector<std::shared_ptr<AggregationFn>> agg_fns);
    auto beforeFirst() -> void override;
    auto next() -> bool override;
    auto getInt(const std::string& fld_name) -> int override;
    auto getString(const std::string& fld_name) -> std::string override;
    auto getVal(const std::string& fld_name) -> Constant override;
    auto hasField(const std::string& fld_name) -> bool override;
    auto close() -> void override;

private:
    std::shared_ptr<Scan> s;
    std::vector<std::string> group_fields;
    std::vector<std::shared_ptr<AggregationFn>> agg_fns;
    std::unique_ptr<GroupValue> group_val;
    bool more_groups;
};

#endif  // CDB_GROUPBYSCAN_H
