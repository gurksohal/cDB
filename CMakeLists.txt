cmake_minimum_required(VERSION 3.22)
project(cDB)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=address,undefined -g")
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/b514bdc898e2951020cbdca1304b75f5950d1f59.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE LIB_SOURCES src/*.cpp src/*.h)
file(GLOB_RECURSE TEST_SOURCES test/*.cpp)

enable_testing()
# lib (src files)
add_library(cDB_lib ${LIB_SOURCES})

# cli
add_executable(cDB_main src/main.cpp)
target_link_libraries(cDB_main cDB_lib)

# create student db
add_executable(cDB_studentDB
        src/CreateStudentDB.cpp)
target_link_libraries(cDB_studentDB cDB_lib)

# All tests
add_executable(cDB_tests ${TEST_SOURCES})

target_link_libraries(cDB_tests
        gtest
        gtest_main
        cDB_lib
)

include(GoogleTest)
gtest_discover_tests(cDB_tests)
