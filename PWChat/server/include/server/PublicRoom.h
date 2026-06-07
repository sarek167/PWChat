#ifndef PUBLICROOM_H
#define PUBLICROOM_H

#include "Room.h"
#include <string>
#include <stdint.h>

/**
 * @class PublicRoom
 * @brief Represents a public chat room that anyone can join.
 * * It inherits from the base Room class. Since it is public,
 * it does not require any passwords or access codes.
 */
class PublicRoom : public Room {
public:
    /**
     * @brief Constructor for PublicRoom class.
     * Passes the room details straight to the base Room constructor.
     * @param id Unique ID of the room.
     * @param name Name of the room.
     * @param ownerId ID of the user who created this room.
     */
    PublicRoom(uint32_t id, std::string name, uint32_t ownerId) : Room(id, name, ownerId) {}

    /**
     * @brief Destructor for PublicRoom class.
     */
    virtual ~PublicRoom() = default;

    /**
     * @brief Checks if a user can join this room.
     * Since this is a public room, it always lets users in.
     * @param token Unused password token (ignored for public rooms).
     * @return Always true, as public rooms don't have restrictions.
     */
    bool canJoin(const std::string& token) override;
};

#endif // PUBLICROOM_H
