#ifndef MESSAGEDATA_H
#define MESSAGEDATA_H

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <stdint.h>
#include <string>

/**
 * @enum MessageContentType
 * @brief Classification enum defining the actual data payload type inside a message.
 */
enum class MessageContentType : uint8_t {
    TEXT = 0,  /**< Standard text paragraph message content. */
    AUDIO = 1  /**< Voice message binary stream payload or asset reference pointer. */
};

/**
 * @struct MessageData
 * @brief Data transfer object (DTO) struct representing a single message instance.
 * * This structure encompasses all necessary identifiers, usernames, text payload data,
 * and structural type specifications required to route and render a message correctly.
 */
struct MessageData {
    uint32_t senderId;              /**< Unique numerical identification index of the message author. */
    std::string senderName;         /**< Display profile nickname. */
    uint32_t targetId;              /**< Destination identifier, representing either a recipient user or a chat room ID. */
    std::string message;            /**< The actual message string contents. */
    MessageContentType messageType = MessageContentType::TEXT; /**< Format classification indicator of the enclosed payload. */

    /**
     * @brief Template method used by Cereal to pack or unpack data.
     * @tparam Archive Type of the archive (e.g., binary, XML, JSON).
     * @param archive Reference to the archive object performing the input or output operation.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(senderId, senderName, targetId, message, messageType);
    }
};

#endif // MESSAGEDATA_H
