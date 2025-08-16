#pragma once
#include "common/types.h"
#include <cstring>

namespace mokshith {

// Page layout:
// | Header (PageHeader) | Free Space | Tuple Data... | Slot Array |
class Page {
public:
    struct PageHeader {
        page_id_t page_id;
        lsn_t page_lsn;
        uint16_t free_space_pointer;  // Points to start of free space
        uint16_t tuple_count;
        uint16_t free_space;
        page_id_t prev_page_id;  // For linked lists
        page_id_t next_page_id;
    };
    
    struct Slot {
        uint16_t offset;  // Offset from page start
        uint16_t length;  // Tuple length (0 = deleted)
    };
    
    Page();
    ~Page() = default;
    
    void Init(page_id_t page_id);
    page_id_t GetPageId() const;
    
    // Tuple operations
    bool InsertTuple(const char* tuple_data, size_t size, slot_offset_t& slot);
    bool DeleteTuple(slot_offset_t slot);
    bool UpdateTuple(slot_offset_t slot, const char* tuple_data, size_t size);
    char* GetTupleData(slot_offset_t slot);
    
    // Page info
    size_t GetFreeSpace() const;
    uint16_t GetTupleCount() const;
    
private:
    char data_[PAGE_SIZE];
    
    PageHeader* GetHeader();
    const PageHeader* GetHeader() const;
    Slot* GetSlot(slot_offset_t slot);
};

} // namespace mokshith