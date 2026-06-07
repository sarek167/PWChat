#ifndef ROOMREQUEST_H
#define ROOMREQUEST_H
#include <string>
#include <cereal/types/string.hpp>

/**
 * @struct CreateRoomRequest
 * @brief Struct used to send a request to create a new chat room.
 * * It contains the basic settings needed by the server to set up,
 * configure, and assign ownership of the new room.
 */
struct CreateRoomRequest {
    std::string roomName; /**< The chosen name for the new chat room. */
    bool isPrivate;       /**< Flag showing if the room should be private (requires a password) or public. */
    bool isAdmin;         /**< Flag checking if the creator should automatically get admin rights in this room. */

    /**
     * @brief Serialization method used by the Cereal library to pack or unpack this struct.
     * @tparam Archive Type of the data stream (like binary or JSON).
     * @param archive Reference to the archive object that saves or loads the data fields.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(roomName, isPrivate, isAdmin);
    }
};

#endif // ROOMREQUEST_H
