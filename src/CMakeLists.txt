# Parser generation
FLEX_TARGET(SqlLexer parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
BISON_TARGET(SqlParser parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
ADD_FLEX_BISON_DEPENDENCY(SqlLexer SqlParser)

# Source files
set(COMMON_SOURCES
    common/config.cpp
    common/exception.cpp
    common/logger.cpp
)

set(STORAGE_SOURCES
    storage/page.cpp
    storage/disk_manager.cpp
    storage/buffer_pool.cpp
    storage/table_heap.cpp
)

set(INDEX_SOURCES
    index/btree.cpp
    index/hash_index.cpp
)

set(CATALOG_SOURCES
    catalog/catalog.cpp
    catalog/schema.cpp
    catalog/table_metadata.cpp
)

set(PARSER_SOURCES
    ${FLEX_SqlLexer_OUTPUTS}
    ${BISON_SqlParser_OUTPUTS}
    parser/ast.cpp
    parser/sql_parser.cpp
)

set(PLANNER_SOURCES
    planner/planner.cpp
    planner/optimizer.cpp
    planner/plan_node.cpp
)

set(EXECUTION_SOURCES
    execution/executor_factory.cpp
    execution/operators/seq_scan.cpp
    execution/operators/index_scan.cpp
    execution/operators/insert.cpp
    execution/operators/delete.cpp
    execution/operators/update.cpp
    execution/operators/join.cpp
    execution/operators/aggregate.cpp
)

set(TRANSACTION_SOURCES
    transaction/transaction_manager.cpp
    transaction/lock_manager.cpp
    transaction/log_manager.cpp
    transaction/recovery_manager.cpp
)

set(NETWORK_SOURCES
    network/server.cpp
    network/protocol.cpp
)

# Create library
add_library(mokshith_db_lib STATIC
    ${COMMON_SOURCES}
    ${STORAGE_SOURCES}
    ${INDEX_SOURCES}
    ${CATALOG_SOURCES}
    ${PARSER_SOURCES}
    ${PLANNER_SOURCES}
    ${EXECUTION_SOURCES}
    ${TRANSACTION_SOURCES}
    ${NETWORK_SOURCES}
)

target_link_libraries(mokshith_db_lib
    Threads::Threads
)

# Create executable
add_executable(mokshith_db main.cpp)
target_link_libraries(mokshith_db mokshith_db_lib)