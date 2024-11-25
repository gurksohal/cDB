cmake_minimum_required(VERSION 3.22)
project(cDB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=address,undefined -g")
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
        src/metadata/IndexInfo.cpp
        src/metadata/IndexInfo.h
        src/metadata/IndexMgr.cpp
        src/metadata/IndexMgr.h
        src/metadata/MetadataMgr.cpp
        src/metadata/MetadataMgr.h
        src/metadata/StatInfo.cpp
        src/metadata/StatInfo.h
        src/metadata/StatMgr.cpp
        src/metadata/StatMgr.h
        src/metadata/TableMgr.cpp
        src/metadata/TableMgr.h
        src/metadata/ViewMgr.cpp
        src/metadata/ViewMgr.h
        src/query/Constant.cpp
        src/query/Constant.h
        src/query/Expression.cpp
        src/query/Expression.h
        src/query/Predicate.cpp
        src/query/Predicate.h
        src/query/ProductScan.cpp
        src/query/ProductScan.h
        src/query/ProjectScan.cpp
        src/query/ProjectScan.h
        src/query/Scan.h
        src/query/SelectScan.cpp
        src/query/SelectScan.h
        src/query/Term.cpp
        src/query/Term.h
        src/query/UpdateScan.h
        src/parse/CreateIndexData.h
        src/parse/CreateTableData.h
        src/parse/CreateViewData.h
        src/parse/QueryData.cpp
        src/parse/QueryData.h
        src/parse/DeletaData.h
        src/parse/InsertData.h
        src/parse/Lexer.cpp
        src/parse/Lexer.h
        src/parse/ModifyData.h
        src/parse/Parser.cpp
        src/parse/Parser.h
        src/parse/PredParser.h
        src/plan/QueryPlanner.h
        src/plan/Plan.h
        src/plan/TablePlan.cpp
        src/plan/TablePlan.h
        src/plan/SelectPlan.cpp
        src/plan/SelectPlan.h
        src/plan/ProjectPlan.cpp
        src/plan/ProjectPlan.h
        src/plan/ProductPlan.cpp
        src/plan/ProductPlan.h
        src/plan/OptimizedProductPlan.cpp
        src/plan/OptimizedProductPlan.h
        src/plan/BasicQueryPlanner.cpp
        src/plan/BasicQueryPlanner.h
        src/plan/BetterQueryPlanner.cpp
        src/plan/BetterQueryPlanner.h
        src/plan/UpdatePlanner.h
        src/plan/Planner.cpp
        src/plan/Planner.h
        src/plan/BasicUpdatePlanner.cpp
        src/plan/BasicUpdatePlanner.h
        src/index/Index.h
        src/index/hash/HashIndex.cpp
        src/index/hash/HashIndex.h
        src/index/planner/IndexJoinPlan.cpp
        src/index/planner/IndexJoinPlan.h
        src/index/query/IndexJoinScan.cpp
        src/index/query/IndexJoinScan.h
        src/index/query/IndexSelectScan.cpp
        src/index/query/IndexSelectScan.h
        src/index/planner/IndexSelectPlan.cpp
        src/index/planner/IndexSelectPlan.h
        src/index/planner/IndexUpdatePlanner.cpp
        src/index/planner/IndexUpdatePlanner.h
        src/materialize/AggregationFn.h
        src/materialize/CountFn.cpp
        src/materialize/CountFn.h
        src/materialize/GroupByPlan.cpp
        src/materialize/GroupByPlan.h
        src/materialize/GroupByScan.cpp
        src/materialize/GroupByScan.h
        src/materialize/GroupValue.cpp
        src/materialize/GroupValue.h
        src/materialize/MaterializePlan.cpp
        src/materialize/MaterializePlan.h
        src/materialize/TempTable.cpp
        src/materialize/TempTable.h
        src/materialize/MaxFn.cpp
        src/materialize/MaxFn.h
        src/materialize/MergeJoinPlan.cpp
        src/materialize/MergeJoinPlan.h
        src/materialize/MergeJoinScan.cpp
        src/materialize/MergeJoinScan.h
        src/materialize/RecordComparator.h
        src/materialize/SortPlan.cpp
        src/materialize/SortPlan.h
        src/materialize/SortScan.cpp
        src/materialize/SortScan.h
        src/multibuffer/BufferNeeds.cpp
        src/multibuffer/BufferNeeds.h
        src/multibuffer/ChunkScan.cpp
        src/multibuffer/ChunkScan.h
        src/multibuffer/MultiBufferProductPlan.cpp
        src/multibuffer/MultiBufferProductPlan.h
        src/multibuffer/MultiBufferProductScan.cpp
        src/multibuffer/MultiBufferProductScan.h
        src/opt/HeuristicQueryPlanner.cpp
        src/opt/HeuristicQueryPlanner.h
        src/opt/TablePlanner.cpp
        src/opt/TablePlanner.h
        src/server/CDB.cpp
        src/server/CDB.h
)

# cli
add_executable(cDB_main src/main.cpp
)
target_link_libraries(cDB_main cDB_lib)

# create student db
add_executable(cDB_studentDB
        src/CreateStudentDB.cpp)
target_link_libraries(cDB_studentDB cDB_lib)

# All tests
add_executable(cDB_tests
        test/FileTest.cpp
        test/LogTest.cpp
        test/BufferManagerTest.cpp
        test/ConcurrencyTest.cpp
        test/TxTest.cpp
        test/RecordTest.cpp
        test/MetadataTest.cpp
        test/QueryTest.cpp
        test/ParseTest.cpp
        test/PlanTest.cpp
)

target_link_libraries(cDB_tests
        gtest
        gtest_main
        cDB_lib
)

include(GoogleTest)
gtest_discover_tests(cDB_tests)
