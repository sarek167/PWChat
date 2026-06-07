#ifndef JOINROOMREQUEST_H
#define JOINROOMREQUEST_H

#include <cereal/types/string.hpp>
#include <string>
#include <cstdint>

/**
 * @struct JoinRoomRequest
 * @brief Data transfer object (DTO) struct used for requesting to join an existing chat room.
 * * It carries the name of the target room channel and an optional invitation passcode
 * token required to pass the security verification challenge for private spaces.
 */
struct JoinRoomRequest {
    std::string name;     /**< The literal text name or unique title identifier of the target room. */
    uint32_t token = 0;   /**< The numeric passcode or invitation token value required for private rooms. */

    /**
     * @brief Template method used by Cereal to pack or unpack data.
     * @tparam Archive Type of the archive (e.g., binary, XML, JSON).
     * @param archive Reference to the archive object performing the input or output operation.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(name, token);
    }
};

#endif // JOINROOMREQUEST_H
