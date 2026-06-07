#ifndef CHATCONTEXT_H
#define CHATCONTEXT_H

#include <stdint.h>

/**
 * @struct ChatContext
 * @brief Struct used for storing context of currently opened chat.
 */
struct ChatContext {
    /**
     * @enum Type
     * @brief Type enum which contains chat types.
     */
    enum class Type {
        None, /**< Default state when no conversation is active. */
        Room, /**< Active context is a multi-user group chat room. */
        User  /**< Active context is a private direct message conversation. */
    };

    uint32_t id; /**< Unique identifier of chat */
    Type type = Type::None; /**< Type of chat - if it's either User or Room (or None) */

    /**
     * @brief Overloading operator of "==" for checking if whole context struct is the same as the compared one.
     * @param other The another ChatContext instance to compare with.
     * @return bool True if both contexts have identical IDs and types, false otherwise.
     */
    bool operator==(const ChatContext& other) {
        return id == other.id && type == other.type;
    }
};

#endif // CHATCONTEXT_H
