#ifndef SQLITECONNECTOR_H
#define SQLITECONNECTOR_H

#include "DBConnector.h"
#include <string>
#include <vector>
#include <stdint.h>

extern "C" {
#include "sqlite3.h"
}

/**
 * @class SQLiteConnector
 * @brief Specific implementation of DBConnector using the SQLite3 library.
 * * This class handles the actual SQL queries to read and write data
 * into a local SQLite database file.
 */
class SQLiteConnector : public DBConnector {
private:
    sqlite3* m_db = nullptr; /**< Pointer to the open SQLite database structure. */

public:
    /**
     * @brief Default constructor for SQLiteConnector.
     */
    SQLiteConnector() = default;

    /**
     * @brief Destructor that ensures the database connection is closed.
     */
    ~SQLiteConnector() override = default;

    /**
     * @brief Opens a connection to the local SQLite database file.
     * @param dbPath Path to the .db file.
     * @return true if opened successfully, false otherwise.
     */
    bool connect(const std::string& dbPath) override;

    /**
     * @brief Closes the active SQLite database connection.
     */
    void disconnect() override;

    /**
     * @brief Creates tables (users, rooms, messages, etc.) using SQL statements if they don't exist.
     */
    void initializeSchema() override;

    /**
     * @brief Executes a SELECT query to fetch all rooms from the database.
     * @return Vector with data of all existing rooms.
     */
    std::vector<RoomData> getAllRooms() override;

    /**
     * @brief Fetches rooms that a specific user has joined by querying the relation table.
     * @param userId ID of the user.
     * @return Vector of rooms the user belongs to.
     */
    std::vector<RoomData> getUserRooms(const uint32_t userId) override;

    /**
     * @brief Inserts a new row into the rooms table.
     * @param name Name of the room.
     * @param isPrivate Privacy flag (0 or 1 in SQLite).
     * @param ownerId Creator's user ID.
     * @return The auto-incremented ID of the new room row, or -1 if failed.
     */
    int saveRoom(const std::string& name, const bool isPrivate, const uint32_t ownerId) override;

    /**
     * @brief Links a user to a room in the junction/relation table.
     * @param userId ID of the user.
     * @param roomId ID of the room.
     * @param isAdmin True if the user should be a room administrator.
     * @return true if the row was inserted, false otherwise.
     */
    bool saveUserRoom(const uint32_t userId, const uint32_t roomId, bool isAdmin) override;

    /**
     * @brief Deletes a row linking a user to a room (user leaves the room).
     * @param userId ID of the user.
     * @param roomId ID of the room.
     * @return true if successfully deleted, false otherwise.
     */
    bool deleteUserRoom(const uint32_t userId, const uint32_t roomId) override;

    /**
     * @brief Inserts a new user row into the users table.
     * @param nickname Chosen username.
     * @param password Chosen password string.
     * @return The newly generated user ID row, or 0 if nickname already exists.
     */
    uint32_t registerUser(const std::string& nickname, const std::string& password) override;

    /**
     * @brief Selects a user row matching both the nickname and password.
     * @param nickname Entered username.
     * @param password Entered password.
     * @return The user's ID if credentials match, 0 otherwise.
     */
    uint32_t loginUser(const std::string& nickname, const std::string& password) override;

    /**
     * @brief Fetches users belonging to a room, filtering by their admin status if requested.
     * @param roomId ID of the target room.
     * @param getAdmins If true, returns only the admins of the room.
     * @return Vector of matching users.
     */
    std::vector<UserData> getRoomUsers(const uint32_t roomId, bool getAdmins = false) override;

    /**
     * @brief Updates a user's role to admin for a specific room row.
     * @param roomId ID of the room.
     * @param userId ID of the user to promote.
     * @return true if the row was updated, false otherwise.
     */
    bool addAdmin(const uint32_t roomId, const uint32_t userId) override;

    /**
     * @brief Inserts a new message entry into the messages table.
     * @param senderID ID of the author.
     * @param targetId ID of the recipient room or recipient user.
     * @param message Text payload or audio resource reference.
     * @param type Content type flag (TEXT or AUDIO).
     * @param toRoom True if message target is a room, false if it's a direct message.
     * @return true if successfully saved, false otherwise.
     */
    bool saveMessage(const uint32_t senderID, const uint32_t targetId, const std::string& message, const MessageContentType& type, bool toRoom) override;

    /**
     * @brief Selects a limited batch of historical messages using LIMIT and OFFSET clauses.
     * @param targetId ID of the room or the other user.
     * @param senderId ID of the user requesting history.
     * @param fromRoom True if loading from a room, false if from private chat.
     * @param limit Maximum number of rows to return.
     * @param offset Number of rows to skip.
     * @return Vector of historical messages found.
     */
    std::vector<MessageData> getMessages(const uint32_t targetId, const uint32_t senderId, bool fromRoom, const int limit, const int offset) override;

    /**
     * @brief Stores an entry pin code for a private room in the database.
     * @param roomId ID of the room.
     * @param code The numeric password code.
     * @return true if updated/saved successfully, false otherwise.
     */
    bool saveRoomCode(const uint32_t roomId, const uint32_t code) override;

    /**
     * @brief Selects the access code value linked to a specific private room.
     * @param roomId ID of the room.
     * @return The numeric code, or 0 if it doesn't exist.
     */
    uint32_t getRoomCode(const uint32_t roomId) override;

    /**
     * @brief Looks up a user's nickname handle string using their unique ID row.
     * @param userId ID of the user.
     * @return The nickname string, or an empty string if not found.
     */
    std::string getUsername(const uint32_t userId) override;

    /**
     * @brief Fetches a list of users that the current user has sent/received private messages from recently.
     * @param userId ID of the active user.
     * @return Vector of users representing recent chat partners.
     */
    std::vector<UserData> getLastUserPrivChats(const uint32_t userId) override;

    /**
     * @brief Searches for a user row by their exact username string.
     * @param username The nickname handle to look up.
     * @return The user's numerical ID if found, 0 otherwise.
     */
    uint32_t findUserByNick(const std::string username) override;
};

#endif // SQLITECONNECTOR_H
