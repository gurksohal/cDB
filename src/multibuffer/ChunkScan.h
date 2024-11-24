//
// Created by G on 11/24/2024.
//

#ifndef CDB_CHUNKSCAN_H
#define CDB_CHUNKSCAN_H

#include <vector>

#include "../query/Scan.h"
#include "../record/RecordPage.h"

class ChunkScan : public Scan {
public:
    ChunkScan(Transaction* tx, const std::string& file_name, const Layout& layout, int start, int end);
    auto beforeFirst() -> void override;
    auto next() -> bool override;
    auto getInt(const std::string& fld_name) -> int override;
    auto getString(const std::string& fld_name) -> std::string override;
    auto getVal(const std::string& fld_name) -> Constant override;
    auto hasField(const std::string& fld_name) -> bool override;
    auto close() -> void override;

private:
    auto moveToBlock(int blk_num) -> void;
    std::vector<RecordPage> buffs;
    Transaction* tx;
    std::string file_name;
    Layout layout;
    int start_b_num;
    int end_b_num;
    int current_b_num;
    std::unique_ptr<RecordPage> rp;
    int current_slot;
};

#endif  // CDB_CHUNKSCAN_H
