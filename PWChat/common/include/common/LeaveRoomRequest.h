#ifndef LEAVEROOMREQUEST_H
#define LEAVEROOMREQUEST_H

#include <stdint.h>

/**
 * @struct LeaveRoomRequest
 * @brief Struct used to send a request to leave a chat room.
 * * It simply holds the IDs needed by the server to remove a specific user
 * from the active members list of a given room.
 */
struct LeaveRoomRequest {
    uint32_t roomId; /**< ID of the chat room that the user wants to leave. */
    uint32_t userId; /**< ID of the user who is leaving the room. */

    /**
     * @brief Serialization method used by the Cereal library to pack or unpack this struct.
     * @tparam Archive Type of the data stream (like binary or JSON).
     * @param archive Reference to the archive object that saves or loads the data fields.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(roomId, userId);
    }
};

#endif // LEAVEROOMREQUEST_H
