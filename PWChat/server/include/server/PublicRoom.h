#ifndef PUBLICROOM_H
#define PUBLICROOM_H

#include "Room.h"

class PublicRoom : public Room {
public:
    PublicRoom(uint32_t id, std::string name, uint32_t ownerId) : Room(id, name, ownerId) {}
    bool canJoin(const std::string& token) override;
};

#endif // PUBLICROOM_H
