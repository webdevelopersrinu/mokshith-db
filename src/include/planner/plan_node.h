#pragma once
#include "catalog/schema.h"
#include "common/types.h"
#include <memory>
#include <vector>

namespace mokshith {

enum class PlanType {
    INVALID = 0,
    SEQ_SCAN,
    INDEX_SCAN,
    INSERT,
    UPDATE,
    DELETE,
    NESTED_LOOP_JOIN,
    HASH_JOIN,
    AGGREGATE,
    LIMIT,
    PROJECTION
};

class PlanNode {
public:
    PlanNode(PlanType type, std::shared_ptr<Schema> output_schema)
        : type_(type), output_schema_(output_schema) {}
    
    virtual ~PlanNode() = default;
    
    PlanType GetType() const { return type_; }
    std::shared_ptr<Schema> GetOutputSchema() const { return output_schema_; }
    
    const std::vector<std::shared_ptr<PlanNode>>& GetChildren() const {
        return children_;
    }
    
    void AddChild(std::shared_ptr<PlanNode> child) {
        children_.push_back(child);
    }
    
protected:
    PlanType type_;
    std::shared_ptr<Schema> output_schema_;
    std::vector<std::shared_ptr<PlanNode>> children_;
};

class SeqScanPlan : public PlanNode {
public:
    SeqScanPlan(std::shared_ptr<Schema> output_schema, 
                const std::string& table_name,
                const std::string& table_alias,
                oid_t table_oid,
                const Expression* predicate)
        : PlanNode(PlanType::SEQ_SCAN, output_schema),
          table_name_(table_name),
          table_alias_(table_alias),
          table_oid_(table_oid),
          predicate_(predicate) {}
    
    const std::string& GetTableName() const { return table_name_; }
    oid_t GetTableOid() const { return table_oid_; }
    const Expression* GetPredicate() const { return predicate_; }
    
private:
    std::string table_name_;
    std::string table_alias_;
    oid_t table_oid_;
    const Expression* predicate_;
};

class InsertPlan : public PlanNode {
public:
    InsertPlan(std::shared_ptr<Schema> output_schema,
               std::shared_ptr<PlanNode> child,
               oid_t table_oid,
               const std::vector<std::vector<Value>>& values)
        : PlanNode(PlanType::INSERT, output_schema),
          table_oid_(table_oid),
          values_(values) {
        if (child) AddChild(child);
    }
    
    oid_t GetTableOid() const { return table_oid_; }
    const std::vector<std::vector<Value>>& GetValues() const { return values_; }
    
private:
    oid_t table_oid_;
    std::vector<std::vector<Value>> values_;
};

} // namespace mokshith