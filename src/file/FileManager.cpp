//
// Created by G on 10/16/2024.
//

#include "FileManager.h"

#include <iostream>

FileManager::FileManager(std::filesystem::path& dir_path, int block_size)
    : dir_path(dir_path), block_size(block_size), is_new(!std::filesystem::exists(dir_path)) {
    if (is_new) {
        std::filesystem::create_directory(dir_path);
    }

    // remove tmp files
    for (const auto& file : std::filesystem::directory_iterator {dir_path}) {
        auto filename = file.path().filename();
        if (filename.string().starts_with("temp")) {
            auto res = std::filesystem::remove(file.path());
            if (!res) {
                std::cerr << "Couldn't delete " << file.path() << "\n";
                throw std::runtime_error("Can't delete file");
            }
        }
    }
}

auto FileManager::read(const BlockId& block_id, Page& page) -> void {
    const std::unique_lock<std::mutex> lock(mutex_lock);

    auto& open_file = getFile(block_id.filename());
    auto page_offset = block_id.blockNumber() * block_size;
    open_file.seekg(page_offset, std::ios::beg);
    std::vector<std::byte> bytes(block_size);
    open_file.read(reinterpret_cast<char*>(bytes.data()), block_size);
    page.replaceData(bytes);
}

auto FileManager::write(BlockId& block_id, Page& page) -> void {
    const std::unique_lock<std::mutex> lock(mutex_lock);

    auto& open_file = getFile(block_id.filename());
    auto page_offset = block_id.blockNumber() * block_size;
    open_file.seekg(page_offset, std::ios::beg);
    open_file.write(reinterpret_cast<const char*>(page.getData().data()), block_size);
}

auto FileManager::append(std::string& filename) -> BlockId {
    std::unique_lock<std::mutex> const lock(mutex_lock);

    auto& open_file = getFile(filename);
    int const new_block_num = length(filename);
    BlockId block_id {filename, new_block_num};
    auto page_offset = block_id.blockNumber() * block_size;
    std::vector<std::byte> bytes(block_size);
    open_file.seekg(page_offset, std::ios::beg);
    open_file.write(reinterpret_cast<const char*>(bytes.data()), block_size);
    return block_id;
}

auto FileManager::isNew() const -> bool {
    return is_new;
}

auto FileManager::length(std::string& filename) -> int {
    auto dir_file_path = dir_path / std::filesystem::path {filename};
    int const size = std::max(0, static_cast<int>(std::filesystem::file_size(dir_file_path)));
    auto new_index = size / block_size;
    return new_index;
}

auto FileManager::blockSize() const -> int {
    return block_size;
}

auto FileManager::getFile(const std::string_view filename) -> std::fstream& {
    auto dir_file_path = dir_path / std::filesystem::path {filename};
    if (open_files.contains(dir_file_path)) {
        std::fstream& ret = open_files[dir_file_path];
        if (ret.is_open()) {
            return ret;
        }
    }

    std::fstream ret {dir_file_path.string(), std::ios::binary | std::ios::in | std::ios::out};
    if (!ret.is_open()) {
        ret.open(dir_file_path.string(), std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
        ret.close();
        ret.open(dir_file_path.string(), std::ios::binary | std::ios::in | std::ios::out);
        if (!ret.is_open()) {
            std::cerr << "Unable to open file: " << filename << "\n";
            throw std::runtime_error("Can't open file");
        }
    }

    open_files[dir_file_path] = std::move(ret);
    std::fstream& f = open_files[dir_file_path];
    return f;
}
