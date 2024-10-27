//
// Created by G on 10/26/2024.
//

#include "../src/buffer/BufferManager.h"
#include "gtest/gtest.h"

namespace {
auto makeFM() -> FileManager {
    std::filesystem::path path = std::filesystem::path {"bufferfile"};
    return {path, 400};
}

auto makeLM(FileManager& fm) -> LogManager {
    std::string const log_file_path = "bufferfile/logtestfile";
    std::ofstream const file(log_file_path);
    return {&fm, "logtestfile"};
}

auto cleanUp() -> void {
    auto r = std::remove("bufferfile/logtestfile");
    EXPECT_EQ(r, 0);
    r = std::remove("bufferfile/testfile");
    EXPECT_EQ(r, 0);
    r = std::remove("bufferfile");
    EXPECT_EQ(r, 0);
}

auto getBlockID(int block) -> BlockId {
    BlockId id {"testfile", block};
    return id;
}
}  // namespace

TEST(bufferfile, bufferfile_test) {
    auto fm = makeFM();
    auto lm = makeLM(fm);
    BufferManager bp {&fm, &lm, 8};

    BlockId blk {"testfile", 2};
    int const pos1 = 88;

    auto& b1 = bp.pin(blk);
    Page* p1 = b1.contents();
    p1->setString(pos1, "abcdefghijklm");
    auto size = Page::maxLength(static_cast<int>(std::string("abcdefghijklm").size()));
    auto pos2 = pos1 + size;
    p1->setInt(pos2, 345);
    b1.setModified(1, 0);
    bp.unpin(b1);

    Buffer& b2 = bp.pin(blk);
    Page* p2 = b2.contents();
    EXPECT_EQ(p2->getInt(pos2), 345);
    EXPECT_EQ(p2->getString(pos1), "abcdefghijklm");
    bp.unpin(b2);

    cleanUp();
}

TEST(buffer, buffer_test) {
    auto fm = makeFM();
    auto lm = makeLM(fm);
    BufferManager bp {&fm, &lm, 3};
    BlockId blk {"testfile", 1};

    Buffer& buff1 = bp.pin(blk);
    Page* p = buff1.contents();
    buff1.setModified(1, 0);
    int const n = p->getInt(0);
    p->setInt(0, n + 1);
    EXPECT_EQ(p->getInt(0), n + 1);
    buff1.setModified(1, 0);
    bp.unpin(buff1);

    blk = {"testfile", 2};
    auto& buff2 = bp.pin(blk);
    blk = {"testfile", 3};
    auto& buff3 = bp.pin(blk);
    blk = {"testfile", 4};
    auto& buff4 = bp.pin(blk);

    bp.unpin(buff2);

    blk = {"testfile", 1};
    buff2 = bp.pin(blk);
    cleanUp();
}

TEST(buffermgr, buffermgr_test) {
    auto fm = makeFM();
    auto lm = makeLM(fm);
    BufferManager bp {&fm, &lm, 3};
    std::vector<Buffer*> buffers;
    auto b0 = getBlockID(0);
    auto b1 = getBlockID(1);
    auto b2 = getBlockID(2);
    auto b3 = getBlockID(3);

    buffers.push_back(&bp.pin(b0));
    buffers.push_back(&bp.pin(b1));
    buffers.push_back(&bp.pin(b2));

    bp.unpin(*buffers[1]);
    buffers[1] = nullptr;
    buffers.push_back(&bp.pin(b0));
    buffers.push_back(&bp.pin(b1));

    EXPECT_EQ(bp.available(), 0);
    EXPECT_ANY_THROW(bp.pin(b3));
    cleanUp();
}