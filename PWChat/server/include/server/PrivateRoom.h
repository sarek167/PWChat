#ifndef PRIVATEROOM_H
#define PRIVATEROOM_H

#include "Room.h"
#include <string>
#include <stdint.h>

/**
 * @class PrivateRoom
 * @brief Represents a private chat room that requires an access code to join.
 * * It inherits from the base Room class and adds a password/pin protection system.
 */
class PrivateRoom : public Room {
private:
    std::string m_accessCode; /**< The password or pin code needed to enter this room. */

public:
    /**
     * @brief Constructor for PrivateRoom class.
     * Passes fundamental room parameters straight to the base Room constructor.
     * @param id Unique ID of the room.
     * @param name Name of the room.
     * @param ownerId ID of the user who created this room.
     */
    PrivateRoom(uint32_t id, std::string name, uint32_t ownerId) : Room(id, name, ownerId) {}

    /**
     * @brief Destructor for PrivateRoom class.
     */
    virtual ~PrivateRoom() = default;

    /**
     * @brief Checks if the provided code matches the room's access code.
     * @param token The password/pin entered by the user.
     * @return true if the code is correct and user can join, false otherwise.
     */
    bool canJoin(const std::string& token) override;
};

#endif // PRIVATEROOM_H
