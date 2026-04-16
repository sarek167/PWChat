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
};

#endif // SQLITECONNECTOR_H
