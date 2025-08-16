#pragma once
#include "storage/page.h"
#include "storage/disk_manager.h"
#include "common/types.h"
#include <unordered_map>
#include <list>
#include <mutex>
#include <atomic>

namespace mokshith {

class BufferPool {
public:
    BufferPool(size_t pool_size, DiskManager* disk_manager);
    ~BufferPool();
    
    // Page operations
    Page* FetchPage(page_id_t page_id);
    bool UnpinPage(page_id_t page_id, bool is_dirty);
    bool FlushPage(page_id_t page_id);
    Page* NewPage(page_id_t& page_id);
    bool DeletePage(page_id_t page_id);
    void FlushAllPages();
    
private:
    struct Frame {
        Page page;
        page_id_t page_id;
        std::atomic<int> pin_count;
        bool is_dirty;
    };
    
    // LRU eviction
    class LRUReplacer {
    public:
        explicit LRUReplacer(size_t num_pages);
        bool Victim(frame_id_t* frame_id);
        void Pin(frame_id_t frame_id);
        void Unpin(frame_id_t frame_id);
        size_t Size();
        
    private:
        std::mutex mutex_;
        std::list<frame_id_t> lru_list_;
        std::unordered_map<frame_id_t, std::list<frame_id_t>::iterator> lru_map_;
    };
    
    size_t pool_size_;
    Frame* frames_;
    DiskManager* disk_manager_;
    std::unordered_map<page_id_t, frame_id_t> page_table_;
    std::list<frame_id_t> free_list_;
    LRUReplacer* replacer_;
    std::mutex latch_;
    
    frame_id_t GetVictimFrame();
};

} // namespace mokshith