#include <gtest/gtest.h>
#include "storage/buffer_pool.h"
#include "storage/disk_manager.h"

using namespace mokshith;

class BufferPoolTest : public ::testing::Test {
protected:
    void SetUp() override {
        disk_manager_ = new DiskManager("test.db");
        buffer_pool_ = new BufferPool(10, disk_manager_);
    }
    
    void TearDown() override {
        delete buffer_pool_;
        delete disk_manager_;
        std::remove("test.db");
    }
    
    DiskManager* disk_manager_;
    BufferPool* buffer_pool_;
};

TEST_F(BufferPoolTest, FetchNewPage) {
    page_id_t page_id;
    Page* page = buffer_pool_->NewPage(page_id);
    
    ASSERT_NE(page, nullptr);
    EXPECT_EQ(page->GetPageId(), page_id);
    
    buffer_pool_->UnpinPage(page_id, false);
}

TEST_F(BufferPoolTest, FetchExistingPage) {
    page_id_t page_id;
    Page* page1 = buffer_pool_->NewPage(page_id);
    ASSERT_NE(page1, nullptr);
    
    strcpy(page1->GetData(), "Test Data");
    buffer_pool_->UnpinPage(page_id, true);
    
    Page* page2 = buffer_pool_->FetchPage(page_id);
    ASSERT_NE(page2, nullptr);
    EXPECT_STREQ(page2->GetData(), "Test Data");
    
    buffer_pool_->UnpinPage(page_id, false);
}

TEST_F(BufferPoolTest, LRUEviction) {
    std::vector<page_id_t> page_ids;
    
    // Fill buffer pool
    for (size_t i = 0; i < 10; ++i) {
        page_id_t page_id;
        Page* page = buffer_pool_->NewPage(page_id);
        ASSERT_NE(page, nullptr);
        page_ids.push_back(page_id);
        buffer_pool_->UnpinPage(page_id, false);
    }
    
    // This should evict the first page
    page_id_t new_page_id;
    Page* new_page = buffer_pool_->NewPage(new_page_id);
    ASSERT_NE(new_page, nullptr);
    
    // First page should no longer be in buffer pool
    Page* evicted_page = buffer_pool_->FetchPage(page_ids[0]);
    ASSERT_NE(evicted_page, nullptr);  // But can still fetch from disk
}