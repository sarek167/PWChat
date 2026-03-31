#ifndef PUBLICROOM_H
#define PUBLICROOM_H

#include "Room.h"

class PublicRoom : public Room {
public:
    PublicRoom(uint32_t id, std::string name) : Room(id, name) {}
    bool canJoin(const std::string& token) override;
};

#endif // PUBLICROOM_H
