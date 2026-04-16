#ifndef SQLITECONNECTOR_H
#define SQLITECONNECTOR_H

#include "DBConnector.h"
extern "C" {
    #include "sqlite3.h"
}

class SQLiteConnector : public DBConnector {
private:
    sqlite3* m_db = nullptr;

public:
    bool connect(const std::string& dbPath) override;
    void disconnect() override;
    void initializeSchema() override;
    std::vector<RoomData> getAllRooms() override;
    int saveRoom(const std::string& name, const bool isPrivate, const uint32_t ownerId) override;
};

#endif // SQLITECONNECTOR_H
