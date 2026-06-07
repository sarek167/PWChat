#ifndef LEAVEROOMREQUEST_H
#define LEAVEROOMREQUEST_H

#include <stdint.h>

/**
 * @struct LeaveRoomRequest
 * @brief Data transfer object (DTO) struct used for requesting to leave a chat room.
 * * This structure carries the necessary context identifiers to disconnect and unregister
 * a specific user session from a given group conversation channel database matrix.
 */
struct LeaveRoomRequest {
    uint32_t roomId; /**< Unique identifier of the target chat room to leave. */
    uint32_t userId; /**< Unique identifier of the user who is departing the room. */

    /**
     * @brief Template method used by Cereal to pack or unpack data.
     * @tparam Archive Type of the archive (e.g., binary, XML, JSON).
     * @param archive Reference to the archive object performing the input or output operation.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(roomId, userId);
    }
};

#endif // LEAVEROOMREQUEST_H
