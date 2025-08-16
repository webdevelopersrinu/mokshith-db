#pragma once
#include "common/types.h"
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <atomic>

namespace mokshith {

enum class TransactionState {
    GROWING,
    SHRINKING,
    COMMITTED,
    ABORTED
};

enum class IsolationLevel {
    READ_UNCOMMITTED,
    READ_COMMITTED,
    REPEATABLE_READ,
    SERIALIZABLE
};

class Transaction {
public:
    explicit Transaction(txn_id_t txn_id, 
                        IsolationLevel isolation_level = IsolationLevel::READ_COMMITTED)
        : txn_id_(txn_id),
          state_(TransactionState::GROWING),
          isolation_level_(isolation_level),
          prev_lsn_(INVALID_LSN) {}
    
    txn_id_t GetTransactionId() const { return txn_id_; }
    TransactionState GetState() const { return state_; }
    IsolationLevel GetIsolationLevel() const { return isolation_level_; }
    
    void SetState(TransactionState state) { state_ = state; }
    
    // Lock management
    void AddSharedLock(const RID& rid) { shared_lock_set_.insert(rid); }
    void AddExclusiveLock(const RID& rid) { exclusive_lock_set_.insert(rid); }
    void RemoveSharedLock(const RID& rid) { shared_lock_set_.erase(rid); }
    void RemoveExclusiveLock(const RID& rid) { exclusive_lock_set_.erase(rid); }
    
    const std::unordered_set<RID>& GetSharedLockSet() const { return shared_lock_set_; }
    const std::unordered_set<RID>& GetExclusiveLockSet() const { return exclusive_lock_set_; }
    
    // Write set for rollback
    void AddToWriteSet(const WriteRecord& record) { write_set_.push_back(record); }
    const std::vector<WriteRecord>& GetWriteSet() const { return write_set_; }
    
    // LSN for recovery
    lsn_t GetPrevLSN() const { return prev_lsn_; }
    void SetPrevLSN(lsn_t lsn) { prev_lsn_ = lsn; }
    
private:
    txn_id_t txn_id_;
    std::atomic<TransactionState> state_;
    IsolationLevel isolation_level_;
    
    std::unordered_set<RID> shared_lock_set_;
    std::unordered_set<RID> exclusive_lock_set_;
    std::vector<WriteRecord> write_set_;
    
    lsn_t prev_lsn_;
};

class TransactionManager {
public:
    TransactionManager(LockManager* lock_manager, LogManager* log_manager);
    ~TransactionManager();
    
    Transaction* Begin(IsolationLevel isolation_level = IsolationLevel::READ_COMMITTED);
    void Commit(Transaction* txn);
    void Abort(Transaction* txn);
    
private:
    std::atomic<txn_id_t> next_txn_id_;
    std::unordered_map<txn_id_t, std::unique_ptr<Transaction>> txn_map_;
    LockManager* lock_manager_;
    LogManager* log_manager_;
    std::mutex txn_map_latch_;
};

} // namespace mokshith