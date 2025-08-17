#pragma once
#include <cstdint>
#include <string>

namespace mokshith {
    using page_id_t = int32_t;
    using frame_id_t = int32_t;
    using txn_id_t = int32_t;
    
    static constexpr size_t PAGE_SIZE = 4096;
    static constexpr size_t BUFFER_POOL_SIZE = 100;
}
