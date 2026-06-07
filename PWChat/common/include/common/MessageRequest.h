#ifndef MESSAGEREQUEST_H
#define MESSAGEREQUEST_H

#include <cereal/archives/binary.hpp>
#include <stdint.h>

/**
 * @struct MessageRequest
 * @brief Data transfer object (DTO) struct used for requesting chat history logs from the server.
 * * This structure defines the queries and offsets needed to page and load historical message
 * batches dynamically for both private conversations and multi-user chat rooms.
 */
struct MessageRequest {
    uint32_t targetId; /**< Unique identifier of the target chat room or user whose history is being fetched. */
    uint32_t offset;   /**< The current list page displacement boundary used to pull the next batch of older messages. */
    bool fromRoom;     /**< Flag specifying if the history should be fetched from a room channel (true) or private chat (false). */

    /**
     * @brief Template method used by Cereal to pack or unpack data.
     * @tparam Archive Type of the archive (e.g., binary, XML, JSON).
     * @param archive Reference to the archive object performing the input or output operation.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(targetId, offset, fromRoom);
    }
};

#endif // MESSAGEREQUEST_H
