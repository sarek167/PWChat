#ifndef PRIVATEROOM_H
#define PRIVATEROOM_H

#include "Room.h"

class PrivateRoom : public Room {
private:
    std::string m_accessCode;

public:
    bool canJoin(const std::string& token) override;
};

#endif // PRIVATEROOM_H
