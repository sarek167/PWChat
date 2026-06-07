#ifndef PACKETHEADER_H
#define PACKETHEADER_H

#include <stdint.h>
#include "MessageType.h"

static_assert(true);
#pragma pack(push)
#pragma pack(1)

/**
 * @struct PacketHeader
 * @brief Fixed-size header placed at the beginning of every network packet.
 * * It uses 1-byte alignment to make sure the structure looks exactly the same
 * in memory on both the client and the server when sending data over sockets.
 */
struct PacketHeader {
    uint32_t signature = 0x50574348;              /**< Magic bytes used to verify that the packet comes from our app (ASCII for 'PWCH'). */
    MessageType type = MessageType::MESS_TO_USER; /**< Type of the message, telling the server which command should handle this packet. */
    uint32_t targetId = 0;                         /**< ID of the target room or the user who should receive this packet. */
    uint32_t senderId = 0;                         /**< ID of the user who sent this packet. */
    uint32_t bodySize = 0;                         /**< Size of the extra data (the body) attached right after this header, in bytes. */
};
#pragma pack(pop)
#endif // PACKETHEADER_H
