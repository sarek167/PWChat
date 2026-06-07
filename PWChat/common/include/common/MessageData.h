#ifndef MESSAGEDATA_H
#define MESSAGEDATA_H

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <stdint.h>
#include <string>

/**
 * @enum MessageContentType
 * @brief Simple enum to check if the message is a text or an audio file.
 */
enum class MessageContentType : uint8_t {
    TEXT = 0,  /**< Standard text message. */
    AUDIO = 1  /**< Voice message / audio data. */
};

/**
 * @struct MessageData
 * @brief Struct that holds all details about a single chat message.
 * * It stores the sender's info, the target destination, the message content itself,
 * and its type so the client knows how to display it.
 */
struct MessageData {
    uint32_t senderId;              /**< ID of the user who sent the message. */
    std::string senderName;         /**< Nickname of the user who sent the message. */
    uint32_t targetId;              /**< ID of the receiver (either a specific user or a chat room). */
    std::string message;            /**< The actual content of the message (text or audio filename/data). */
    MessageContentType messageType = MessageContentType::TEXT; /**< Tells if the message is text or audio. */

    /**
     * @brief Serialization method used by the Cereal library to pack or unpack this struct.
     * @tparam Archive Type of the data stream (like binary or JSON).
     * @param archive Reference to the archive object that saves or loads the data fields.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(senderId, senderName, targetId, message, messageType);
    }
};

#endif // MESSAGEDATA_H
