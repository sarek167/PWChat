#ifndef PRIVATEROOM_H
#define PRIVATEROOM_H

#include "Room.h"

class PrivateRoom : public Room {
private:
    std::string m_accessCode;

public:
    PrivateRoom(uint32_t id, std::string name, uint32_t ownerId) : Room(id, name, ownerId) {}
    bool canJoin(const std::string& token) override;
};

#endif // PRIVATEROOM_H
