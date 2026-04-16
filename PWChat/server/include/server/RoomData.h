#ifndef ROOMDATA_H
#define ROOMDATA_H

#include <stdint.h>
#include <string>

struct RoomData {
    uint32_t id;
    std::string name;
    bool isPrivate;
    uint32_t ownerId;
};

#endif // ROOMDATA_H
