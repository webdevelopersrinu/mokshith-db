enable_testing()

# Google Test
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/googletest)
include_directories(${gtest_SOURCE_DIR}/include)

# Test files
set(TEST_SOURCES
    unit/storage/page_test.cpp
    unit/storage/buffer_pool_test.cpp
    unit/index/btree_test.cpp
    unit/catalog/catalog_test.cpp
    unit/parser/parser_test.cpp
    unit/execution/executor_test.cpp
    unit/transaction/lock_manager_test.cpp
    integration/sql_test.cpp
    benchmark/performance_test.cpp
)

# Create test executable
add_executable(mokshith_test ${TEST_SOURCES})
target_link_libraries(mokshith_test
    mokshith_db_lib
    gtest
    gtest_main
)

# Add tests
add_test(NAME mokshith_test COMMAND mokshith_test)