#ifndef CHATCONTEXT_H
#define CHATCONTEXT_H

#include <stdint.h>

struct ChatContext {
    enum class Type {None, Room, Private};
    uint32_t id;
    Type type = Type::None;

    bool operator==(const ChatContext& other) {
        return id == other.id && type == other.type;
    }
};

#endif // CHATCONTEXT_H
