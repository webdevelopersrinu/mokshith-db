#pragma once
#include "storage/disk_manager.h"
#include <atomic>
#include <condition_variable>
#include <future>
#include <queue>

namespace mokshith {

enum class LogRecordType {
    BEGIN,
    COMMIT,
    ABORT,
    INSERT,
    DELETE,
    UPDATE,
    CHECKPOINT
};

struct LogRecord {
    LogRecordType type;
    txn_id_t txn_id;
    lsn_t lsn;
    lsn_t prev_lsn;
    
    // For data modification
    RID rid;
    page_id_t page_id;
    
    union {
        struct {  // INSERT
            size_t insert_size;
            char insert_data[0];
        };
        struct {  // DELETE
            size_t delete_size;
            char delete_data[0];
        };
        struct {  // UPDATE
            size_t old_size;
            size_t new_size;
            char update_data[0];  // old data followed by new data
        };
    };
    
    size_t GetSize() const;
    void SerializeTo(char* buffer) const;
    void DeserializeFrom(const char* buffer);
};

class LogManager {
public:
    LogManager(DiskManager* disk_manager);
    ~LogManager();
    
    lsn_t AppendLogRecord(const LogRecord& log_record);
    
    void Flush(lsn_t lsn);
    void FlushAll();
    
    // Recovery
    void Redo();
    void Undo();
    
private:
    DiskManager* disk_manager_;
    
    char* log_buffer_;
    size_t log_buffer_size_;
    std::atomic<size_t> log_buffer_offset_;
    
    std::atomic<lsn_t> next_lsn_;
    std::atomic<lsn_t> persistent_lsn_;
    
    std::mutex latch_;
    std::condition_variable cv_flush_;
    std::thread* flush_thread_;
    std::atomic<bool> enable_flushing_;
    
    void RunFlushThread();
    void SwapLogBuffer();
};

} // namespace mokshith