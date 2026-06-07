#ifndef ROOMUSERDATA_H
#define ROOMUSERDATA_H

#include "common/UserData.h"
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <stdint.h>
#include <string>

/**
 * @struct RoomUserData
 * @brief Struct that holds detailed information about a chat room.
 * * It packages everything needed to display a room, including its settings,
 * the list of regular members, and the list of room administrators.
 */
struct RoomUserData {
    uint32_t id;                  /**< Unique ID number of the chat room. */
    std::string name;             /**< The display name of the room. */
    bool isPrivate;               /**< Flag showing if the room is private (true) or public (false). */
    uint32_t accessCode = 0;      /**< The numeric password/pin needed to enter a private room. */
    std::vector<UserData> users;  /**< List of standard users currently in the room. */
    std::vector<UserData> admins; /**< List of users who have admin privileges in this room. */

    /**
     * @brief Serialization method used by the Cereal library to pack or unpack this struct.
     * @tparam Archive Type of the data stream (like binary or JSON).
     * @param archive Reference to the archive object that saves or loads the room data fields.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(id, name, isPrivate, accessCode, users, admins);
    }
};

#endif // ROOMUSERDATA_H
