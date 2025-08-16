#pragma once
#include "transaction/log_manager.h"
#include "transaction/transaction_manager.h"
#include <unordered_map>
#include <unordered_set>

namespace mokshith {

class RecoveryManager {
public:
    RecoveryManager(LogManager* log_manager,
                   TransactionManager* txn_manager,
                   BufferPool* buffer_pool);
    
    void StartRecovery();
    void Checkpoint();
    
private:
    // ARIES recovery phases
    void Analysis();
    void Redo();
    void Undo();
    
    LogManager* log_manager_;
    TransactionManager* txn_manager_;
    BufferPool* buffer_pool_;
    
    // Recovery state
    std::unordered_map<txn_id_t, lsn_t> active_txn_table_;
    std::unordered_map<page_id_t, lsn_t> dirty_page_table_;
    lsn_t checkpoint_lsn_;
    
    void RedoOperation(const LogRecord& record);
    void UndoOperation(const LogRecord& record);
};

} // namespace mokshith