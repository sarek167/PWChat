#ifndef ROOMDATA_H
#define ROOMDATA_H

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <stdint.h>
#include <string>

/**
 * @struct RoomData
 * @brief Simple struct used to hold basic chat room information.
 * * It contains the essential settings and IDs needed to synchronize
 * room details between the server and the clients.
 */
struct RoomData {
    uint32_t id;          /**< Unique ID number of the chat room. */
    std::string name;     /**< The display name of the room. */
    bool isPrivate;       /**< Flag showing if the room is private (true) or public (false). */
    uint32_t ownerId;     /**< Unique ID number of the user who created/owns the room. */

    /**
     * @brief Serialization method used by the Cereal library to pack or unpack this struct.
     * @tparam Archive Type of the data stream (like binary or JSON).
     * @param archive Reference to the archive object that saves or loads the room data fields.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(id, name, isPrivate, ownerId);
    }
};

#endif // ROOMDATA_H
