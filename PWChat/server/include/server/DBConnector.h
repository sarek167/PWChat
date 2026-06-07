#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include <string>
#include <stdint.h>
#include <vector>
#include "common/RoomData.h"
#include "common/UserData.h"
#include "common/MessageData.h"

/**
 * @class DBConnector
 * @brief Abstract interface for handling database operations.
 * * Defines all the methods needed to save and load application data
 * like users, rooms, and chat messages.
 */
class DBConnector {
public:
    /**
     * @brief Virtual destructor to ensure proper cleanup in derived classes.
     */
    virtual ~DBConnector() = default;

    /**
     * @brief Opens a connection to the database file.
     * @param dbPath Path to the database file (e.g., "database.db").
     * @return true if connected successfully, false otherwise.
     */
    virtual bool connect(const std::string& dbPath) = 0;

    /**
     * @brief Closes the active database connection.
     */
    virtual void disconnect() = 0;

    /**
     * @brief Creates necessary tables in the database if they don't exist yet.
     */
    virtual void initializeSchema() = 0;

    /**
     * @brief Fetches all available chat rooms from the database.
     * @return A vector containing data of all rooms.
     */
    virtual std::vector<RoomData> getAllRooms() = 0;

    /**
     * @brief Fetches all rooms that a specific user has joined.
     * @param userId ID of the user.
     * @return A vector of rooms the user belongs to.
     */
    virtual std::vector<RoomData> getUserRooms(const uint32_t userId) = 0;

    /**
     * @brief Saves a newly created room to the database.
     * @param name Name of the room.
     * @param isPrivate True if the room requires an access code.
     * @param ownerId ID of the user who created the room.
     * @return The ID of the newly created room, or -1 if failed.
     */
    virtual int saveRoom(const std::string& name, const bool isPrivate, const uint32_t ownerId) = 0;

    /**
     * @brief Adds a user to a specific room (links them together).
     * @param userId ID of the user.
     * @param roomId ID of the room.
     * @param isAdmin True if the user should have admin rights in this room.
     * @return true if saved successfully, false otherwise.
     */
    virtual bool saveUserRoom(const uint32_t userId, const uint32_t roomId, bool isAdmin) = 0;

    /**
     * @brief Removes a user from a specific room (e.g., when they leave).
     * @param userId ID of the user.
     * @param roomId ID of the room.
     * @return true if removed successfully, false otherwise.
     */
    virtual bool deleteUserRoom(const uint32_t userId, const uint32_t roomId) = 0;

    /**
     * @brief Registers a new user account.
     * @param nickname Chosen username.
     * @param password Chosen password (preferably hashed).
     * @return The newly generated user ID, or 0 if failed (e.g., nickname taken).
     */
    virtual uint32_t registerUser(const std::string& nickname, const std::string& password) = 0;

    /**
     * @brief Checks user credentials during login.
     * @param nickname Entered username.
     * @param password Entered password.
     * @return The user ID if credentials are correct, 0 otherwise.
     */
    virtual uint32_t loginUser(const std::string& nickname, const std::string& password) = 0;

    /**
     * @brief Gets a list of members belonging to a specific room.
     * @param roomId ID of the room.
     * @param getAdmins If true, fetches only room administrators; otherwise fetches regular users.
     * @return A vector of users in the room.
     */
    virtual std::vector<UserData> getRoomUsers(const uint32_t roomId, bool getAdmins = false) = 0;

    /**
     * @brief Promotes a standard user to an admin in a specific room.
     * @param roomId ID of the room.
     * @param userId ID of the user to promote.
     * @return true if successful, false otherwise.
     */
    virtual bool addAdmin(const uint32_t roomId, const uint32_t userId) = 0;

    /**
     * @brief Saves a sent message (text or audio) to the history log.
     * @param senderID ID of the user who sent it.
     * @param targetId ID of the receiver (either room ID or another user's ID).
     * @param message Text content of the message or file path for audio data.
     * @param type Type of content (TEXT or AUDIO).
     * @param toRoom True if sent to a group room, false if it's a private message.
     * @return true if message was saved, false otherwise.
     */
    virtual bool saveMessage(const uint32_t senderID, const uint32_t targetId, const std::string& message, const MessageContentType& type, bool toRoom) = 0;

    /**
     * @brief Loads a limited batch of archived messages for a chat history window.
     * @param targetId ID of the room or the other user.
     * @param senderId ID of the current user asking for history.
     * @param fromRoom True if loading from a room, false if from private chat.
     * @param limit How many messages to load at once (pagination).
     * @param offset How many messages to skip (for scrolling up).
     * @return A vector of found messages.
     */
    virtual std::vector<MessageData> getMessages(const uint32_t targetId, const uint32_t senderId, bool fromRoom, const int limit, const int offset) = 0;

    /**
     * @brief Saves an invitation/entry pin code for a private room.
     * @param roomId ID of the room.
     * @param code The numeric password.
     * @return true if saved successfully, false otherwise.
     */
    virtual bool saveRoomCode(const uint32_t roomId, const uint32_t code) = 0;

    /**
     * @brief Fetches the invitation pin code assigned to a private room.
     * @param roomId ID of the room.
     * @return The numeric code, or 0 if room has no code or is public.
     */
    virtual uint32_t getRoomCode(const uint32_t roomId) = 0;

    /**
     * @brief Looks up a user's nickname using their unique ID.
     * @param userId ID of the user.
     * @return The nickname string, or empty string if not found.
     */
    virtual std::string getUsername(const uint32_t userId) = 0;

    /**
     * @brief Gets a list of users with whom the current user had recent private chats.
     * @param userId ID of the current user.
     * @return A vector of users representing recent direct conversations.
     */
    virtual std::vector<UserData> getLastUserPrivChats(const uint32_t userId) = 0;

    /**
     * @brief Searches for a user in the database by their exact username.
     * @param username The nickname to look for.
     * @return The user's ID if found, 0 otherwise.
     */
    virtual uint32_t findUserByNick(const std::string username) = 0;
};

#endif // DBCONNECTOR_H
