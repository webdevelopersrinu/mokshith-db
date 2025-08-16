#pragma once
#include "storage/buffer_pool.h"
#include <vector>
#include <list>

namespace mokshith {

template <typename KeyType, typename ValueType, typename HashFunc>
class HashIndex {
public:
    HashIndex(BufferPool* buffer_pool, 
              const HashFunc& hash_func,
              size_t bucket_count = 1024);
    
    bool Insert(const KeyType& key, const ValueType& value, txn_id_t txn_id);
    bool Remove(const KeyType& key, txn_id_t txn_id);
    bool GetValue(const KeyType& key, std::vector<ValueType>& result);
    
private:
    struct HashBucket {
        std::list<std::pair<KeyType, ValueType>> entries;
        std::mutex latch;
    };
    
    BufferPool* buffer_pool_;
    HashFunc hash_func_;
    std::vector<std::unique_ptr<HashBucket>> buckets_;
    size_t bucket_count_;
    
    size_t GetBucketIndex(const KeyType& key) {
        return hash_func_(key) % bucket_count_;
    }
};

} // namespace mokshith