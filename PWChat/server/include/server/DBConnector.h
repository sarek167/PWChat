#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include <string>
#include <stdint.h>
#include <vector>
#include "common/RoomData.h"
#include "common/UserData.h"

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
    virtual uint32_t registerUser(const std::string& nickname, const std::string& password) = 0;
    virtual uint32_t loginUser(const std::string& nickname, const std::string& password) = 0;
    virtual std::vector<UserData>getRoomUsers(const uint32_t roomId, bool getAdmins = false) = 0;
};

#endif // DBCONNECTOR_H
