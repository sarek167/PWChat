#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <stdint.h>
#include "Room.h"
#include "common/RoomData.h"

class Session;

/**
 * @class RoomManager
 * @brief Class that manages all chat rooms on the server.
 * * It allows creating, finding, and removing rooms, as well as updating
 * room lists when users log in or out.
 */
class RoomManager {
public:
    /**
     * @brief Getter for the map of all existing rooms.
     * @return A map where the key is the room ID and the value is a shared pointer to the Room.
     */
    std::map<uint32_t, std::shared_ptr<Room>> allRooms();

    /**
     * @brief Finds a room by its unique numerical ID.
     * @param id The ID of the room to search for.
     * @return Shared pointer to the found Room, or nullptr if it doesn't exist.
     */
    std::shared_ptr<Room> getRoom(uint32_t id);

    /**
     * @brief Finds a room by its name string.
     * @param name The name of the room to search for.
     * @return Shared pointer to the found Room, or nullptr if it doesn't exist.
     */
    std::shared_ptr<Room> getRoom(std::string name);

    /**
     * @brief Creates a new room instance (either Public or Private) and adds it to the manager.
     * @param roomId Unique ID for the new room.
     * @param name Name of the room.
     * @param isPrivate True if it should be a PrivateRoom, false for a PublicRoom.
     * @param ownerId ID of the user who created this room.
     * @return Shared pointer to the newly created Room object.
     */
    std::shared_ptr<Room> createRoom(uint32_t roomId, std::string name, bool isPrivate, uint32_t ownerId);

    /**
     * @brief Deletes a room from the manager using its name.
     * @param name Name of the room to be removed.
     */
    void removeRoom(std::string name);

    /**
     * @brief Initializes the manager with a list of rooms (e.g., loaded from the database at startup).
     * @param rooms Vector of room data structures to build the internal maps.
     */
    void initialize(const std::vector<RoomData>& rooms);

    /**
     * @brief Links a user's session to all the rooms they belong to when they log in.
     * @param rooms Vector of rooms that the user is a member of.
     * @param session Shared pointer to the user's active network session.
     */
    void loginInitialize(const std::vector<RoomData>& rooms, const std::shared_ptr<Session> session);

    /**
     * @brief Unlinks a user's session from their rooms when they disconnect or log out.
     * @param rooms Vector of rooms that the user is leaving.
     * @param session Shared pointer to the user's active network session.
     */
    void logoutInitialize(const std::vector<RoomData>& rooms, const std::shared_ptr<Session> session);

private:
    std::map<uint32_t, std::shared_ptr<Room>> m_allRooms;          /**< Map for quick room lookups by their numerical ID. */
    std::map<std::string, std::shared_ptr<Room>> m_allRoomsByName; /**< Map for quick room lookups by their text name. */
};

#endif // ROOMMANAGER_H
