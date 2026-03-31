#ifndef PRIVATEROOM_H
#define PRIVATEROOM_H

#include "Room.h"

class PrivateRoom : public Room {
private:
    std::string m_accessCode;

public:
    PrivateRoom(uint32_t id, std::string name) : Room(id, name) {}
    bool canJoin(const std::string& token) override;
};

#endif // PRIVATEROOM_H
