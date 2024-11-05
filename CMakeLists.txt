cmake_minimum_required(VERSION 3.22)
project(cDB)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/b514bdc898e2951020cbdca1304b75f5950d1f59.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
# lib (src files)
add_library(cDB_lib
        src/file/BlockId.cpp
        src/file/BlockId.h
        src/file/FileManager.cpp
        src/file/FileManager.h
        src/file/Page.cpp
        src/file/Page.h
        src/log/LogManager.cpp
        src/log/LogManager.h
        src/log/LogIterator.cpp
        src/log/LogIterator.h
        src/buffer/Buffer.cpp
        src/buffer/Buffer.h
        src/buffer/BufferManager.cpp
        src/buffer/BufferManager.h
        src/tx/BufferList.cpp
        src/tx/BufferList.h
        src/tx/Transaction.cpp
        src/tx/Transaction.h
        src/tx/concurrency/ConcurrencyManager.cpp
        src/tx/concurrency/ConcurrencyManager.h
        src/tx/concurrency/LockTable.cpp
        src/tx/concurrency/LockTable.h
        src/tx/recovery/LogRecord.cpp
        src/tx/recovery/LogRecord.h
        src/tx/recovery/RecoveryManager.cpp
        src/tx/recovery/RecoveryManager.h
        src/record/Layout.cpp
        src/record/Layout.h
        src/record/RID.cpp
        src/record/RID.h
        src/record/RecordPage.cpp
        src/record/RecordPage.h
        src/record/Schema.cpp
        src/record/Schema.h
        src/record/TableScan.cpp
        src/record/TableScan.h
)

# single main file for now, scratchpad
add_executable(cDB_main src/main.cpp
)
target_link_libraries(cDB_main cDB_lib)

# All tests
add_executable(cDB_tests
        test/FileTest.cpp
        test/LogTest.cpp
        test/BufferManagerTest.cpp
        test/ConcurrencyTest.cpp
        test/TxTest.cpp
)

target_link_libraries(cDB_tests
        gtest
        gtest_main
        cDB_lib
)

include(GoogleTest)
gtest_discover_tests(cDB_tests)
