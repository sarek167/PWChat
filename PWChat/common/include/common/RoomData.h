#ifndef ROOMDATA_H
#define ROOMDATA_H

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <stdint.h>
#include <string>

/**
 * @struct RoomData
 * @brief Data transfer object (DTO) struct representing a chat room's basic metadata properties.
 * * This structure encompasses the vital configurations, identifiers, ownership mappings,
 * and structural parameters required to synchronize room instances between the server and clients.
 */
struct RoomData {
    uint32_t id;          /**< Unique numerical identifier of the chat room channel. */
    std::string name;     /**< The display name or title label assigned to the room. */
    bool isPrivate;       /**< Flag specifying if the room is passcode-protected (true) or open to the public (false). */
    uint32_t ownerId;     /**< Unique numerical identifier of the user who holds root creator/ownership rights. */

    /**
     * @brief Template method used by Cereal to pack or unpack data.
     * @tparam Archive Type of the archive (e.g., binary, XML, JSON).
     * @param archive Reference to the archive object performing the input or output operation.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(id, name, isPrivate, ownerId);
    }
};

#endif // ROOMDATA_H
