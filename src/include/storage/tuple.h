#pragma once
#include "common/types.h"
#include "catalog/schema.h"
#include <vector>

namespace mokshith {

// Tuple format:
// | Header | Null Bitmap | Column Data... |
class Tuple {
public:
    Tuple() = default;
    Tuple(std::vector<Value> values, const Schema* schema);
    
    // Serialize/Deserialize
    void SerializeTo(char* storage) const;
    void DeserializeFrom(const char* storage);
    
    Value GetValue(const Schema* schema, uint32_t column_idx) const;
    void SetValue(const Schema* schema, uint32_t column_idx, const Value& value);
    
    size_t GetSize() const { return size_; }
    const char* GetData() const { return data_; }
    
private:
    size_t size_;
    char* data_;
    bool allocated_;
};

class TableHeap {
public:
    TableHeap(BufferPool* buffer_pool, const Schema* schema);
    ~TableHeap();
    
    // Tuple operations
    bool InsertTuple(const Tuple& tuple, txn_id_t txn_id);
    bool DeleteTuple(const RID& rid, txn_id_t txn_id);
    bool UpdateTuple(const Tuple& tuple, const RID& rid, txn_id_t txn_id);
    bool GetTuple(const RID& rid, Tuple& tuple, txn_id_t txn_id);
    
    // Iterator for sequential scan
    class Iterator {
    public:
        Iterator(TableHeap* table_heap, RID rid);
        const Tuple& operator*();
        Iterator& operator++();
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
        
    private:
        TableHeap* table_heap_;
        RID current_rid_;
        Tuple current_tuple_;
    };
    
    Iterator Begin(txn_id_t txn_id);
    Iterator End();
    
private:
    BufferPool* buffer_pool_;
    const Schema* schema_;
    page_id_t first_page_id_;
    page_id_t last_page_id_;
};

} // namespace mokshith