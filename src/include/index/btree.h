#pragma once
#include "storage/page.h"
#include "storage/buffer_pool.h"
#include <queue>
#include <algorithm>

namespace mokshith {

template <typename KeyType, typename ValueType, typename ComparatorType>
class BPlusTree {
public:
    BPlusTree(const std::string& name, 
              BufferPool* buffer_pool,
              const ComparatorType& comparator,
              int max_size = 64);
    
    // Main operations
    bool Insert(const KeyType& key, const ValueType& value, txn_id_t txn_id);
    bool Remove(const KeyType& key, txn_id_t txn_id);
    bool GetValue(const KeyType& key, std::vector<ValueType>& result);
    
    // Iterator for range scans
    class Iterator {
    public:
        Iterator(BPlusTree* tree, page_id_t page_id, int index);
        
        const std::pair<KeyType, ValueType>& operator*();
        Iterator& operator++();
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
        
    private:
        BPlusTree* tree_;
        page_id_t page_id_;
        int index_;
        std::pair<KeyType, ValueType> current_;
        
        void FetchCurrent();
    };
    
    Iterator Begin();
    Iterator End();
    Iterator Begin(const KeyType& key);
    
private:
    // Node types
    enum class NodeType : uint8_t {
        LEAF = 0,
        INTERNAL = 1
    };
    
    // Base node structure
    struct BPlusTreePage {
        NodeType node_type;
        int size;
        int max_size;
        page_id_t parent_page_id;
        page_id_t page_id;
    };
    
    // Internal node
    struct InternalPage : public BPlusTreePage {
        page_id_t child_page_ids[0];  // Flexible array
        KeyType keys[0];               // Flexible array
        
        void Init(page_id_t page_id, page_id_t parent_id, int max_size);
        bool Insert(const KeyType& key, page_id_t child_id, const ComparatorType& comp);
        page_id_t Lookup(const KeyType& key, const ComparatorType& comp);
    };
    
    // Leaf node
    struct LeafPage : public BPlusTreePage {
        page_id_t next_page_id;
        KeyType keys[0];       // Flexible array
        ValueType values[0];   // Flexible array
        
        void Init(page_id_t page_id, page_id_t parent_id, int max_size);
        bool Insert(const KeyType& key, const ValueType& value, const ComparatorType& comp);
        bool Remove(const KeyType& key, const ComparatorType& comp);
        bool Lookup(const KeyType& key, ValueType& value, const ComparatorType& comp);
    };
    
    // Helper methods
    Page* FetchPage(page_id_t page_id);
    Page* NewPage(page_id_t& page_id);
    
    bool InsertIntoLeaf(const KeyType& key, const ValueType& value, 
                       txn_id_t txn_id, LeafPage* leaf);
    bool SplitLeaf(LeafPage* leaf, const KeyType& key, const ValueType& value);
    bool InsertIntoParent(page_id_t old_page, const KeyType& key, 
                         page_id_t new_page, txn_id_t txn_id);
    
    template <typename N>
    bool CoalesceOrRedistribute(N* node, txn_id_t txn_id);
    
    page_id_t root_page_id_;
    BufferPool* buffer_pool_;
    ComparatorType comparator_;
    int max_size_;
    std::mutex latch_;
};

} // namespace mokshith