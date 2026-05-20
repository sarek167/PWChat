#ifndef MESSAGEDATA_H
#define MESSAGEDATA_H

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <stdint.h>
#include <string>

enum class MessageContentType : uint8_t {
    TEXT = 0,
    AUDIO = 1,
};

struct MessageData {


    uint32_t senderId;
    uint32_t targetId;
    std::string message;
    MessageContentType messageType = MessageContentType::TEXT;


    template<class Archive>
    void serialize(Archive & archive) {
        uint8_t& typeRef = reinterpret_cast<uint8_t&>(messageType);
        archive(senderId, targetId, message, messageType);
    }
};

#endif // MESSAGEDATA_H
