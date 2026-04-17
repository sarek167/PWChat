#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include <string>
#include <stdint.h>
#include <vector>
#include "server/RoomData.h"

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
};

#endif // DBCONNECTOR_H
