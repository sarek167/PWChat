#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include <string>
#include <stdint.h>
#include <vector>
#include "common/RoomData.h"
#include "common/UserData.h"
#include "common/MessageData.h"

class DBConnector {
public:
    virtual ~DBConnector() = default;
    virtual bool connect(const std::string& dbPath) = 0;
    virtual void disconnect() = 0;
    virtual void initializeSchema() = 0;
    virtual std::vector<RoomData> getAllRooms() = 0;
    virtual std::vector<RoomData> getUserRooms(const uint32_t userId) = 0;
    virtual int saveRoom(const std::string& name, const bool isPrivate, const uint32_t ownerId) = 0;
    virtual bool saveUserRoom(const uint32_t userId, const uint32_t roomId, bool isAdmin) = 0;
    virtual bool deleteUserRoom(const uint32_t userId, const uint32_t roomId) = 0;
    virtual uint32_t registerUser(const std::string& nickname, const std::string& password) = 0;
    virtual uint32_t loginUser(const std::string& nickname, const std::string& password) = 0;
    virtual std::vector<UserData>getRoomUsers(const uint32_t roomId, bool getAdmins = false) = 0;
    virtual bool addAdmin(const uint32_t roomId, const uint32_t userId) = 0;
    virtual bool saveMessage(const uint32_t senderID, const uint32_t targetId, const std::string& message, const MessageContentType& type, bool toRoom) = 0;
    virtual std::vector<MessageData> getMessages(const uint32_t targetId, const uint32_t senderId, bool fromRoom, const int limit, const int offset) = 0;
};

#endif // DBCONNECTOR_H
