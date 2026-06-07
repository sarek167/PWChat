#ifndef MESSAGEREQUEST_H
#define MESSAGEREQUEST_H

#include <cereal/archives/binary.hpp>
#include <stdint.h>

/**
 * @struct MessageRequest
 * @brief Struct used to ask the server for older chat messages.
 * * It contains the information needed to load a specific batch of past messages
 * (pagination) for either a private chat or a room.
 */
struct MessageRequest {
    uint32_t targetId; /**< ID of the room or user whose chat history we want to load. */
    uint32_t offset;   /**< The number of messages to skip (offset) to load the next older batch. */
    bool fromRoom;     /**< Flag showing if we are loading messages from a room (true) or a private chat (false). */

    /**
     * @brief Serialization method used by the Cereal library to pack or unpack this struct.
     * @tparam Archive Type of the data stream (like binary or JSON).
     * @param archive Reference to the archive object that saves or loads the data fields.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(targetId, offset, fromRoom);
    }
};

#endif // MESSAGEREQUEST_H
