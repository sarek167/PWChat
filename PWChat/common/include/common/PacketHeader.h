#ifndef PACKETHEADER_H
#define PACKETHEADER_H

#include <stdint.h>
#include "MessageType.h"

static_assert(true);
#pragma pack(push)
#pragma pack(1)
struct PacketHeader {
    uint32_t signature = 0x50574348;
    MessageType type = MessageType::TEXT_TO_USER;
    uint32_t targetId = 0;
    uint32_t senderId = 0;
    uint32_t bodySize = 0;
};
#pragma pack(pop)
#endif // PACKETHEADER_H
