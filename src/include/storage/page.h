#pragma once
#include "common/types.h"

namespace mokshith {
    class Page {
    public:
        Page();
        ~Page() = default;
        
        void Init(page_id_t page_id);
        page_id_t GetPageId() const { return page_id_; }
        
    private:
        char data_[PAGE_SIZE];
        page_id_t page_id_;
    };
}
