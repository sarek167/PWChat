#ifndef PACKETHEADER_H
#define PACKETHEADER_H

#include <stdint.h>
#include "MessageType.h"

struct PacketHeader {
    uint32_t signature = 0x50574348;
    MessageType type = MessageType::TEXT_TO_USER;
    uint32_t targetId = 0;
    uint32_t senderId = 0;
    uint32_t bodySize = 0;

    template<class Archive>
    void serialize(Archive& archive) {
        archive(signature, type, targetId, senderId, bodySize);
    };
};

#endif // PACKETHEADER_H
