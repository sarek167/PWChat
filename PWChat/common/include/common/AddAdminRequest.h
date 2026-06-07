#ifndef ADDADMINREQUEST_H
#define ADDADMINREQUEST_H

#include <stdint.h>

/**
 * @struct AddAdminRequest
 * @brief Struct used to send a request to promote a user to a room administrator.
 * * It simply holds the IDs needed by the server to grant admin privileges
 * to a specific user within a specific chat room.
 */
struct AddAdminRequest {
    uint32_t roomId; /**< ID of the chat room where the promotion takes place. */
    uint32_t userId; /**< ID of the user who is being promoted to admin. */

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

#endif // ADDADMINREQUEST_H
