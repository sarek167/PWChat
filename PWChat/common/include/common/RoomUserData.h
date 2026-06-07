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
 * @brief Data transfer object (DTO) struct containing detailed room properties and member rosters.
 * * Extends basic room descriptors by carrying active membership collections,
 * administrative operator groupings, and privacy access key codes used during joining rooms.
 */
struct RoomUserData {
    uint32_t id;                  /**< Unique numerical identifier of the chat room channel. */
    std::string name;             /**< The display name or title label assigned to the room. */
    bool isPrivate;               /**< Flag specifying if the room is passcode-protected (true) or open to the public (false). */
    uint32_t accessCode = 0;      /**< The active numeric pin or verification invitation code required to pass entry blocks. */
    std::vector<UserData> users;  /**< Vector container storing user profile rows representing the regular room audience roster. */
    std::vector<UserData> admins; /**< Vector dataset caching user descriptions of moderators holding operator credentials. */

    /**
     * @brief Template method used by Cereal to pack or unpack data.
     * @tparam Archive Type of the archive (e.g., binary, XML, JSON).
     * @param archive Reference to the archive object performing the input or output operation.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(id, name, isPrivate, accessCode, users, admins);
    }
};

#endif // ROOMUSERDATA_H
