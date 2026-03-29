#ifndef PUBLICROOM_H
#define PUBLICROOM_H

#include "Room.h"

class PublicRoom : public Room {
public:
    bool canJoin(const std::string& token) override;
};

#endif // PUBLICROOM_H
