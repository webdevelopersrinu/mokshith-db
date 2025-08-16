#pragma once
#include "catalog/schema.h"
#include "catalog/table_metadata.h"
#include "catalog/index_metadata.h"
#include <unordered_map>
#include <memory>
#include <mutex>

namespace mokshith {

class Catalog {
public:
    Catalog(BufferPool* buffer_pool);
    ~Catalog();
    
    // Table operations
    bool CreateTable(txn_id_t txn_id,
                    const std::string& table_name,
                    const Schema& schema);
    
    bool DropTable(txn_id_t txn_id,
                  const std::string& table_name);
    
    TableMetadata* GetTable(const std::string& table_name);
    TableMetadata* GetTable(oid_t table_oid);
    
    // Index operations
    bool CreateIndex(txn_id_t txn_id,
                    const std::string& index_name,
                    const std::string& table_name,
                    const std::vector<uint32_t>& key_columns,
                    IndexType index_type);
    
    bool DropIndex(txn_id_t txn_id,
                  const std::string& index_name);
    
    IndexMetadata* GetIndex(const std::string& index_name);
    IndexMetadata* GetIndex(oid_t index_oid);
    
    std::vector<IndexMetadata*> GetTableIndexes(const std::string& table_name);
    
private:
    BufferPool* buffer_pool_;
    
    // Tables
    std::unordered_map<oid_t, std::unique_ptr<TableMetadata>> tables_;
    std::unordered_map<std::string, oid_t> table_names_;
    std::atomic<oid_t> next_table_oid_;
    
    // Indexes
    std::unordered_map<oid_t, std::unique_ptr<IndexMetadata>> indexes_;
    std::unordered_map<std::string, oid_t> index_names_;
    std::unordered_map<oid_t, std::vector<oid_t>> table_indexes_;
    std::atomic<oid_t> next_index_oid_;
    
    std::mutex catalog_latch_;
};

} // namespace mokshith