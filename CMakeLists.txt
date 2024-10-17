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
)

# single main file for now, scratchpad
add_executable(cDB_main src/main.cpp
)
target_link_libraries(cDB_main cDB_lib)

# All tests
add_executable(cDB_tests
        test/file/FileTest.cpp
)

target_link_libraries(cDB_tests
        gtest
        gtest_main
        cDB_lib
)

include(GoogleTest)
gtest_discover_tests(cDB_tests)
