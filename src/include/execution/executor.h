#pragma once
#include "planner/plan_node.h"
#include "storage/tuple.h"
#include "execution/execution_context.h"

namespace mokshith {

class Executor {
public:
    Executor(ExecutionContext* exec_ctx, std::shared_ptr<PlanNode> plan)
        : exec_ctx_(exec_ctx), plan_(plan) {}
    
    virtual ~Executor() = default;
    
    virtual void Init() = 0;
    virtual bool Next(Tuple* tuple) = 0;
    
protected:
    ExecutionContext* exec_ctx_;
    std::shared_ptr<PlanNode> plan_;
};

class SeqScanExecutor : public Executor {
public:
    SeqScanExecutor(ExecutionContext* exec_ctx, 
                    std::shared_ptr<SeqScanPlan> plan);
    
    void Init() override;
    bool Next(Tuple* tuple) override;
    
private:
    std::shared_ptr<SeqScanPlan> plan_;
    TableHeap* table_heap_;
    TableHeap::Iterator iter_;
    TableHeap::Iterator end_;
};

class InsertExecutor : public Executor {
public:
    InsertExecutor(ExecutionContext* exec_ctx,
                   std::shared_ptr<InsertPlan> plan,
                   std::unique_ptr<Executor> child_executor);
    
    void Init() override;
    bool Next(Tuple* tuple) override;
    
private:
    std::shared_ptr<InsertPlan> plan_;
    std::unique_ptr<Executor> child_executor_;
    TableHeap* table_heap_;
    bool executed_;
};

class IndexScanExecutor : public Executor {
public:
    IndexScanExecutor(ExecutionContext* exec_ctx,
                      std::shared_ptr<IndexScanPlan> plan);
    
    void Init() override;
    bool Next(Tuple* tuple) override;
    
private:
    std::shared_ptr<IndexScanPlan> plan_;
    BPlusTreeIndex* index_;
    IndexIterator iter_;
    IndexIterator end_;
};

} // namespace mokshith