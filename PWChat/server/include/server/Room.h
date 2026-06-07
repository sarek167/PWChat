#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <memory>
#include <stdint.h>
#include "common/Packet.h"

class Session;
class User;

/**
 * @class Room
 * @brief Base class representing a general chat room on the server.
 * * It manages the list of connected clients, chat room details, and administrators.
 * This class is abstract, meaning specific room types (like Public or Private) must inherit from it.
 */
class Room {
public:
    /**
     * @brief Constructor for Room class.
     * @param id Unique ID of the room.
     * @param name Name of the room.
     * @param ownerId ID of the user who created this room.
     */
    Room(uint32_t id, std::string name, uint32_t ownerId) : m_id(id), m_name(name), m_ownerId(ownerId) {}

    /**
     * @brief Virtual destructor to ensure proper cleanup in derived room classes.
     */
    virtual ~Room() = default;

    /**
     * @brief Getter for the room's unique ID.
     * @return The numerical ID of the room.
     */
    uint32_t id();

    /**
     * @brief Getter for the room's name.
     * @return The name string of the room.
     */
    std::string name();

    /**
     * @brief Getter for the room owner's user ID.
     * @return ID of the room creator/owner.
     */
    uint32_t ownerId();

    /**
     * @brief Adds a new client session to this room (user enters the room).
     * @param clientToAdd Shared pointer to the user's active connection session.
     */
    void addClient(std::shared_ptr<Session> clientToAdd);

    /**
     * @brief Removes a client session from this room (user leaves the room).
     * @param clientToRemove Shared pointer to the connection session that should be removed.
     */
    void removeClient(std::shared_ptr<Session> clientToRemove);

    /**
     * @brief Pure virtual method to check if a user is allowed to join this room.
     * Must be implemented by PublicRoom and PrivateRoom classes.
     * @param token Password token or pin code needed for entry validation.
     * @return true if access is granted, false otherwise.
     */
    virtual bool canJoin(const std::string& token) = 0;

    /**
     * @brief Sends a network packet to everyone currently present in this room.
     * @param p The packet data structure to send (e.g., a new chat message).
     * @param skipSender If true, the packet won't be sent back to the person who triggered it.
     */
    void broadcast(const Packet& p, bool skipSender = true);

    /**
     * @brief Adds a user to the room's administrator list.
     * @param adminId ID of the user to promote.
     */
    void addAdmin(uint32_t adminId);

    /**
     * @brief Removes a user from the room's administrator list.
     * @param adminId ID of the user to demote.
     */
    void removeAdmin(uint32_t adminId);

    /**
     * @brief Checks if a specific user has administrator rights in this room.
     * @param userId ID of the user to check.
     * @return true if the user is an admin, false otherwise.
     */
    bool checkIfAdmin(const uint32_t userId);

protected:
    uint32_t m_id;                                     /**< Unique numerical identifier of the room. */
    std::string m_name;                                /**< Display name or title of the room channel. */
    uint32_t m_ownerId;                                /**< User ID of the primary owner of this room. */
    std::vector<std::shared_ptr<Session>> m_clients;   /**< List of all user sessions currently active inside this room. */
    std::vector<uint32_t> m_adminIds;                  /**< List of user IDs that have moderator/admin privileges here. */
};

#endif // ROOM_H
