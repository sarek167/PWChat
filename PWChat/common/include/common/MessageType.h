#ifndef MESSAGETYPES_H
#define MESSAGETYPES_H

#include <stdint.h>

enum class MessageType : uint8_t {
    TEXT_TO_USER = 0,
    TEXT_TO_ROOM = 1,
    AUDIO_TO_USER = 2,
    AUDIO_TO_ROOM = 3,
    SERVER_COMMAND = 4,
    LOGIN_REQUEST = 5
};

#endif // MESSAGETYPES_H
