//
// Created by G on 10/26/2024.
//

#include "LogManager.h"

LogManager::LogManager(FileManager *fm, const std::string &path)
    : file_manager(fm),
      log_path(path),
      page(file_manager->blockSize()),
      block_id(log_path, 0),
      latest_lsm(0),
      last_saved_lsm(0) {
    auto log_size = file_manager->length(path);
    if (log_size == 0) {
        block_id = appendNewBlock();
    } else {
        block_id = BlockId(log_path, log_size - 1);
        file_manager->read(block_id, page);
    }
}

auto LogManager::append(std::vector<std::byte> &rec) -> int {
    const std::unique_lock<std::mutex> lock(mutex);
    auto boundary = page.getInt(0);
    auto rec_size = static_cast<int>(rec.size());
    auto bytes_needed = rec_size + static_cast<int>(sizeof(int));
    if (boundary - bytes_needed < static_cast<int>(sizeof(int))) {
        flush();
        block_id = appendNewBlock();
        boundary = page.getInt(0);
    }
    auto rec_pos = boundary - bytes_needed;

    page.setBytes(rec_pos, rec);
    page.setInt(0, rec_pos);
    latest_lsm++;
    return latest_lsm;
}

auto LogManager::flush(int lsn) -> void {
    if (lsn >= last_saved_lsm) {
        flush();
    }
}

auto LogManager::appendNewBlock() -> BlockId {
    auto blk = file_manager->append(log_path);
    page.setInt(0, file_manager->blockSize());
    file_manager->write(blk, page);
    return blk;
}

auto LogManager::flush() -> void {
    file_manager->write(block_id, page);
    last_saved_lsm = latest_lsm;
}

auto LogManager::iterator() -> LogIterator {
    flush();
    return {file_manager, block_id};
}
