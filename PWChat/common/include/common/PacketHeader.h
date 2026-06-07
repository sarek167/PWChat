#ifndef PACKETHEADER_H
#define PACKETHEADER_H

#include <stdint.h>
#include "MessageType.h"

static_assert(true);
#pragma pack(push)
#pragma pack(1)

/**
 * @struct PacketHeader
 * @brief Fixed-size metadata envelope tracking routing and validation parameters for network packets.
 * * This structure is packed with byte-level alignment (1-byte packing) to ensure seamless,
 * binary-compatible layout transfers over TCP sockets between different CPU architectures.
 */
struct PacketHeader {
    uint32_t signature = 0x50574348;              /**< Protocol magic bytes identifier used to validate protocol packets (ASCII for 'PWCH'). */
    MessageType type = MessageType::MESS_TO_USER; /**< Operational protocol opcode specifying the command category layout of the following body. */
    uint32_t targetId = 0;                         /**< Unique numerical identifier of the destination conversation channel room or peer client user. */
    uint32_t senderId = 0;                         /**< Unique tracking key identifying the original dispatch source profile instance connection. */
    uint32_t bodySize = 0;                         /**< Exact length classification in bytes of the serialized payload array attached immediately after this header. */
};
#pragma pack(pop)
#endif // PACKETHEADER_H
