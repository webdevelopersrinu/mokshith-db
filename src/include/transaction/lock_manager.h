#pragma once
#include "transaction/transaction.h"
#include <condition_variable>
#include <list>
#include <unordered_map>

namespace mokshith {

enum class LockMode {
    SHARED,
    EXCLUSIVE
};

class LockManager {
public:
    LockManager();
    ~LockManager();
    
    bool LockShared(Transaction* txn, const RID& rid);
    bool LockExclusive(Transaction* txn, const RID& rid);
    bool LockUpgrade(Transaction* txn, const RID& rid);
    bool Unlock(Transaction* txn, const RID& rid);
    
private:
    struct LockRequest {
        txn_id_t txn_id;
        LockMode lock_mode;
        bool granted;
    };
    
    struct LockRequestQueue {
        std::list<LockRequest> request_queue;
        std::condition_variable cv;
        bool upgrading = false;
    };
    
    std::unordered_map<RID, std::unique_ptr<LockRequestQueue>> lock_table_;
    std::mutex lock_table_latch_;
    
    bool GrantLock(Transaction* txn, const RID& rid, LockMode lock_mode);
    void WaitForLock(Transaction* txn, const RID& rid, LockMode lock_mode);
    void ReleaseLock(Transaction* txn, const RID& rid);
    
    // Deadlock detection
    std::thread* deadlock_detection_thread_;
    std::atomic<bool> enable_deadlock_detection_;
    void RunDeadlockDetection();
    bool HasCycle(txn_id_t txn_id, 
                  std::unordered_map<txn_id_t, std::vector<txn_id_t>>& wait_graph);
};

} // namespace mokshith