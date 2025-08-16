#pragma once
#include "parser/ast.h"
#include "planner/plan_node.h"
#include "catalog/catalog.h"

namespace mokshith {

class Planner {
public:
    explicit Planner(Catalog* catalog) : catalog_(catalog) {}
    
    std::shared_ptr<PlanNode> CreatePlan(const AST* ast);
    
private:
    Catalog* catalog_;
    
    std::shared_ptr<PlanNode> CreateSelectPlan(const SelectAST* ast);
    std::shared_ptr<PlanNode> CreateInsertPlan(const InsertAST* ast);
    std::shared_ptr<PlanNode> CreateUpdatePlan(const UpdateAST* ast);
    std::shared_ptr<PlanNode> CreateDeletePlan(const DeleteAST* ast);
    
    Expression* CreateExpression(const ExpressionAST* ast);
};

class Optimizer {
public:
    explicit Optimizer(Catalog* catalog) : catalog_(catalog) {}
    
    std::shared_ptr<PlanNode> Optimize(std::shared_ptr<PlanNode> plan);
    
private:
    Catalog* catalog_;
    
    // Optimization rules
    std::shared_ptr<PlanNode> PushDownPredicate(std::shared_ptr<PlanNode> plan);
    std::shared_ptr<PlanNode> ChooseJoinAlgorithm(std::shared_ptr<PlanNode> plan);
    std::shared_ptr<PlanNode> UseIndexIfAvailable(std::shared_ptr<PlanNode> plan);
};

} // namespace mokshith