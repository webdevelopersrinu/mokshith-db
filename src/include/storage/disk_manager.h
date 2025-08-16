#pragma once
#include "common/types.h"
#include <string>
#include <fstream>
#include <mutex>

namespace mokshith {

class DiskManager {
public:
    explicit DiskManager(const std::string& db_file);
    ~DiskManager();
    
    // Page operations
    void ReadPage(page_id_t page_id, char* page_data);
    void WritePage(page_id_t page_id, const char* page_data);
    page_id_t AllocatePage();
    void DeallocatePage(page_id_t page_id);
    
    // Database info
    size_t GetFileSize() const;
    int GetNumWrites() const { return num_writes_; }
    int GetNumFlushes() const { return num_flushes_; }
    
    void Flush();
    
private:
    std::string db_file_name_;
    std::fstream db_file_;
    page_id_t next_page_id_;
    int num_writes_;
    int num_flushes_;
    std::mutex db_io_mutex_;
};

} // namespace mokshith