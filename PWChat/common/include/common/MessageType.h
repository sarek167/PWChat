#ifndef MESSAGETYPES_H
#define MESSAGETYPES_H

#include <stdint.h>

enum class MessageType : uint8_t {
    TEXT_TO_USER = 0,
    TEXT_TO_ROOM = 1,
    AUDIO_TO_USER = 2,
    AUDIO_TO_ROOM = 3,
    LOGIN_REQUEST = 4,
    AUTH_RESPONSE = 5,
    JOIN_ROOM_COMM = 6,
    LEAVE_ROOM_COMM = 7,
    CREATE_ROOM_COMM = 8,
    DEL_ROOM_COMM = 9,
};

#endif // MESSAGETYPES_H
