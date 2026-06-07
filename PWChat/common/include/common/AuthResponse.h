#ifndef AUTHRESPONSE_H
#define AUTHRESPONSE_H

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <stdint.h>
#include <string>
#include "common/UserData.h"
#include "common/RoomData.h"

/**
 * @struct AuthResponse
 * @brief Data transfer object (DTO) struct containing the server's response to an authentication request.
 * * This structure carries the authenticated user's profile metadata, along with vectors of
 * historical private text conversation logs and subscribed group channel definitions.
 */
struct AuthResponse {
    uint32_t myId;                   /**< Unique numerical identifier assigned to the authenticated user. */
    std::string myNickname;          /**< The distinct display handle or nickname of the authenticated user. */
    std::vector<UserData> userChats; /**< Vector containing list entries of recent active direct peer chats. */
    std::vector<RoomData> userRooms; /**< Vector containing structural metadata profiles of user's active groups. */

    /**
     * @brief Template method used by Cereal to pack or unpack data.
     * @tparam Archive Type of the archive (e.g., binary, XML, JSON).
     * @param archive Reference to the archive object performing the input or output operation.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(myId, myNickname, userChats, userRooms);
    }
};

#endif // AUTHRESPONSE_H
