//
// Created by G on 10/16/2024.
//

#ifndef CDB_FILEMANAGER_H
#define CDB_FILEMANAGER_H

#include <filesystem>
#include <fstream>
#include <mutex>
#include <unordered_map>

#include "BlockId.h"
#include "Page.h"

class FileManager {
public:
    FileManager(std::filesystem::path& dir_path, int block_size);
    auto read(const BlockId& block_id, Page& page) -> void;
    auto write(const BlockId& block_id, Page& page) -> void;
    auto append(std::string& filename) -> BlockId;
    auto isNew() const -> bool;
    auto length(const std::string& filename) -> int;
    auto blockSize() const -> int;

private:
    std::filesystem::path dir_path;
    int block_size;
    bool is_new;
    std::unordered_map<std::string, std::fstream> open_files;
    std::mutex mutex_lock;
    auto getFile(std::string_view filename) -> std::fstream&;
};

#endif  // CDB_FILEMANAGER_H
