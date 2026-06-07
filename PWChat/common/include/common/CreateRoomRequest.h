#ifndef ROOMREQUEST_H
#define ROOMREQUEST_H
#include <string>
#include <cereal/types/string.hpp>

/**
 * @struct CreateRoomRequest
 * @brief Data transfer object (DTO) struct used for requesting the creation of a new chat room.
 * * Contains the baseline parameters required by the server backend to initialize,
 * configure, and assign ownership credentials for a new group channel namespace.
 */
struct CreateRoomRequest {
    std::string roomName; /**< The requested unique text name or title for the new room channel. */
    bool isPrivate;       /**< Flag specifying if the room requires an access code verification challenge. */
    bool isAdmin;         /**< Flag defining if the requesting creator should be granted admin operator rights. */

    /**
     * @brief Template method used by Cereal to pack or unpack data.
     * @tparam Archive Type of the archive (e.g., binary, XML, JSON).
     * @param archive Reference to the archive object performing the input or output operation.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(roomName, isPrivate, isAdmin);
    }
};

#endif // ROOMREQUEST_H
