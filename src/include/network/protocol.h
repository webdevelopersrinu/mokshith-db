#pragma once
#include <string>
#include <vector>

namespace mokshith {

enum class MessageType : uint8_t {
    QUERY = 1,
    RESPONSE,
    ERROR,
    CONNECT,
    DISCONNECT,
    PREPARE,
    EXECUTE,
    BEGIN_TXN,
    COMMIT,
    ROLLBACK
};

struct Message {
    MessageType type;
    uint32_t length;
    std::vector<uint8_t> payload;
    
    void SerializeTo(char* buffer) const;
    void DeserializeFrom(const char* buffer);
};

class ProtocolHandler {
public:
    static Message CreateQueryMessage(const std::string& query);
    static Message CreateResponseMessage(const ResultSet& result);
    static Message CreateErrorMessage(const std::string& error);
    
    static std::string ParseQuery(const Message& msg);
    static ResultSet ParseResponse(const Message& msg);
    static std::string ParseError(const Message& msg);
};

} // namespace mokshith