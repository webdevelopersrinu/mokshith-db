#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace mokshith {

// Basic type definitions
using page_id_t = int32_t;
using frame_id_t = int32_t;
using txn_id_t = int32_t;
using lsn_t = int32_t;
using slot_offset_t = size_t;
using oid_t = uint32_t;

// Invalid values
static constexpr page_id_t INVALID_PAGE_ID = -1;
static constexpr txn_id_t INVALID_TXN_ID = -1;
static constexpr lsn_t INVALID_LSN = -1;

// Page constants
static constexpr size_t PAGE_SIZE = 4096;  // 4KB
static constexpr size_t BUFFER_POOL_SIZE = 100;

// SQL data types
enum class TypeId {
    INVALID = 0,
    BOOLEAN,
    TINYINT,
    SMALLINT,
    INTEGER,
    BIGINT,
    DECIMAL,
    VARCHAR,
    TIMESTAMP
};

// Value class to hold any SQL type
class Value {
public:
    Value() : type_id_(TypeId::INVALID) {}
    explicit Value(TypeId type) : type_id_(type) {}
    
    TypeId GetTypeId() const { return type_id_; }
    
    // Type-specific getters/setters
    int32_t GetInteger() const;
    std::string GetVarchar() const;
    bool GetBoolean() const;
    
    void SetInteger(int32_t val);
    void SetVarchar(const std::string& val);
    void SetBoolean(bool val);
    
private:
    TypeId type_id_;
    union {
        bool boolean_;
        int8_t tinyint_;
        int16_t smallint_;
        int32_t integer_;
        int64_t bigint_;
        double decimal_;
        char varchar_[128];
    } value_;
};

} // namespace mokshith