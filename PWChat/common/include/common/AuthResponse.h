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
 * @brief Struct holding the server's response after a login or registration attempt.
 * * It sends back the user's own profile info along with lists of their
 * recent private chats and active chat rooms.
 */
struct AuthResponse {
    uint32_t myId;                   /**< The unique ID assigned to the logged-in user. */
    std::string myNickname;          /**< The nickname of the logged-in user. */
    std::vector<UserData> userChats; /**< List of users with whom this user has recent private chats. */
    std::vector<RoomData> userRooms; /**< List of chat rooms that the user belongs to. */

    /**
     * @brief Serialization method used by the Cereal library to pack or unpack this struct.
     * @tparam Archive Type of the data stream (like binary or JSON).
     * @param archive Reference to the archive object that saves or loads the data fields.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(myId, myNickname, userChats, userRooms);
    }
};

#endif // AUTHRESPONSE_H
