Mokshith DB - Complete Development Documentation
Table of Contents

Project Overview
Development Environment Setup
Project Structure
Phase-by-Phase Implementation
Build System
Testing Framework
API Documentation


Project Overview
Mokshith DB - A SQL-based relational database management system built from scratch in C/C++.
Core Features

SQL query support (DDL, DML, DQL)
ACID transactions
B+ Tree indexing
Buffer pool management
Write-ahead logging
Concurrent access control

Technology Stack

Language: C++17/20
Build System: CMake
Testing: Google Test
Parser Generator: Flex/Bison
Documentation: Doxygen


Development Environment Setup
Prerequisites Installation
bash# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    gdb \
    valgrind \
    flex \
    bison \
    libgtest-dev \
    doxygen \
    graphviz

# macOS
brew install cmake flex bison googletest doxygen graphviz

# Windows (using MSYS2)
pacman -S mingw-w64-x86_64-gcc \
    mingw-w64-x86_64-cmake \
    mingw-w64-x86_64-gdb \
    flex bison

Project Structure
```
mokshith-db/
├── CMakeLists.txt              # Main build configuration
├── README.md                   # Project documentation
├── LICENSE                     # License file
├── .gitignore                 
├── docs/                       # Documentation
│   ├── design/                # Design documents
│   │   ├── architecture.md
│   │   ├── storage-engine.md
│   │   └── query-processor.md
│   ├── api/                   # API documentation
│   └── tutorials/             # User tutorials
│
├── src/                        # Source code
│   ├── include/               # Header files
│   │   ├── common/
│   │   │   ├── config.h
│   │   │   ├── types.h
│   │   │   ├── exception.h
│   │   │   └── logger.h
│   │   ├── storage/
│   │   │   ├── page.h
│   │   │   ├── disk_manager.h
│   │   │   ├── buffer_pool.h
│   │   │   └── table_heap.h
│   │   ├── index/
│   │   │   ├── index.h
│   │   │   ├── btree.h
│   │   │   └── hash_index.h
│   │   ├── catalog/
│   │   │   ├── catalog.h
│   │   │   ├── schema.h
│   │   │   └── table_metadata.h
│   │   ├── parser/
│   │   │   ├── lexer.h
│   │   │   ├── parser.h
│   │   │   └── ast.h
│   │   ├── planner/
│   │   │   ├── planner.h
│   │   │   ├── optimizer.h
│   │   │   └── plan_node.h
│   │   ├── execution/
│   │   │   ├── executor.h
│   │   │   ├── executor_factory.h
│   │   │   └── operators/
│   │   ├── transaction/
│   │   │   ├── transaction.h
│   │   │   ├── lock_manager.h
│   │   │   └── log_manager.h
│   │   └── network/
│   │       ├── server.h
│   │       └── protocol.h
│   │
│   ├── common/                # Common utilities implementation
│   │   ├── config.cpp
│   │   ├── exception.cpp
│   │   └── logger.cpp
│   │
│   ├── storage/              # Storage engine implementation
│   │   ├── page.cpp
│   │   ├── disk_manager.cpp
│   │   ├── buffer_pool.cpp
│   │   └── table_heap.cpp
│   │
│   ├── index/                # Index implementations
│   │   ├── btree.cpp
│   │   └── hash_index.cpp
│   │
│   ├── catalog/              # System catalog
│   │   ├── catalog.cpp
│   │   ├── schema.cpp
│   │   └── table_metadata.cpp
│   │
│   ├── parser/               # SQL parser
│   │   ├── lexer.l          # Flex file
│   │   ├── parser.y         # Bison file
│   │   ├── ast.cpp
│   │   └── sql_parser.cpp
│   │
│   ├── planner/              # Query planner
│   │   ├── planner.cpp
│   │   ├── optimizer.cpp
│   │   └── plan_node.cpp
│   │
│   ├── execution/            # Execution engine
│   │   ├── executor_factory.cpp
│   │   └── operators/
│   │       ├── seq_scan.cpp
│   │       ├── index_scan.cpp
│   │       ├── insert.cpp
│   │       ├── delete.cpp
│   │       ├── update.cpp
│   │       ├── join.cpp
│   │       └── aggregate.cpp
│   │
│   ├── transaction/          # Transaction management
│   │   ├── transaction_manager.cpp
│   │   ├── lock_manager.cpp
│   │   └── log_manager.cpp
│   │
│   ├── network/              # Network layer
│   │   ├── server.cpp
│   │   └── protocol.cpp
│   │
│   └── main.cpp              # Entry point
│
├── test/                      # Test files
│   ├── unit/                 # Unit tests
│   │   ├── storage/
│   │   ├── index/
│   │   ├── catalog/
│   │   ├── parser/
│   │   └── execution/
│   ├── integration/          # Integration tests
│   └── benchmark/            # Performance benchmarks
│
├── tools/                     # Utility tools
│   ├── shell/                # Interactive shell
│   ├── import/               # Data import tools
│   └── export/               # Data export tools
│
├── scripts/                   # Build and deployment scripts
│   ├── build.sh
│   ├── test.sh
│   └── format.sh
│
└── third_party/              # Third-party libraries
    ├── googletest/
    └── spdlog/
```


Development Timeline & Milestones
Milestone 1: Basic Storage (Month 1-2)
```
✅ Project setup and build system
✅ Common utilities and types
✅ Page structure and disk manager
✅ Buffer pool with LRU eviction
✅ Table heap and tuple format
```

Deliverable: Can store and retrieve tuples from disk

Milestone 2: SQL Support (Month 3-4)
```

✅ SQL lexer and parser (basic queries)
✅ AST representation
✅ Query planner
✅ Basic executor (SeqScan, Insert)
✅ System catalog
```

Deliverable: Can execute simple SELECT, INSERT, CREATE TABLE

Milestone 3: Indexing (Month 5)
```

✅ B+ Tree implementation
✅ Hash index implementation
✅ Index scan executor
✅ Query optimizer with index selection
```

Deliverable: Queries use indexes for better performance

Milestone 4: Transactions (Month 6-7)
```
✅ Transaction manager
✅ Lock manager with 2PL
✅ Write-ahead logging
✅ Recovery manager (ARIES)
```
Deliverable: Full ACID compliance

Milestone 5: Production Ready (Month 8+)
```
✅ Network protocol and server
✅ Client library and shell
✅ Performance optimization
✅ Comprehensive testing
✅ Documentation
```
Deliverable: Production-ready database system


Running & Testing
```
Build Instructions
bash# Clone repository
git clone https://github.com/webdevelopersrinu/mokshith-db.git
cd mokshith-db

# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
make -j$(nproc)

# Run tests
make test

# Generate documentation
make docs
Running the Database
bash# Start server
./mokshith_db --port 5432 --data-dir /path/to/data

# Connect with shell
./tools/shell/mokshith_shell localhost 5432

# Example queries
mokshith> CREATE TABLE users (id INT, name VARCHAR(50), age INT);
mokshith> INSERT INTO users VALUES (1, 'Alice', 30);
mokshith> SELECT * FROM users WHERE age > 25;
```
Next Steps & Advanced Features
Performance Optimizations

Parallel query execution
Join algorithms (hash join, merge join)
Query result caching
Columnar storage option
Compression

Advanced SQL Features

Subqueries and CTEs
Window functions
Stored procedures
Triggers
Views and materialized views

Distributed Features

Replication (master-slave, multi-master)
Sharding
Distributed transactions
Consensus protocols (Raft)

Monitoring & Management

Query profiler
Performance metrics
Admin dashboard
Backup and restore
Point-in-time recovery
