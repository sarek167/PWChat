#include "server/SQLiteConnector.h"
#include <iostream>

bool SQLiteConnector::connect(const std::string& dbPath) {
    int rc = sqlite3_open(dbPath.c_str(), &m_db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error while connecting to SQLite database" << std::endl;
        return false;
    }
    return true;
}

void SQLiteConnector::disconnect() {
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

void SQLiteConnector::initializeSchema() {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    nickname TEXT UNIQUE NOT NULL,"
        "    password_hash TEXT NOT NULL,"
        "    created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        "    );"
        "CREATE TABLE IF NOT EXISTS rooms ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    name TEXT UNIQUE NOT NULL,"
        "    type BOOLEAN DEFAULT 0 NOT NULL," // 0: public, 1: private
        "    access_code TEXT,"
        "    owner_id INTEGER NOT NULL,"
        "    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "    FOREIGN KEY (owner_id) REFERENCES USERS(id)"
        "    );"
        "CREATE TABLE IF NOT EXISTS users_rooms ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    room_id INTEGER NOT NULL,"
        "    user_id INTEGER NOT NULL,"
        "    is_admin BOOLEAN DEFAULT 0,"
        "    FOREIGN KEY (room_id) REFERENCES ROOMS(id) ON DELETE CASCADE,"
        "    FOREIGN KEY (user_id) REFERENCES USERS(id) ON DELETE CASCADE,"
        "    UNIQUE(room_id, user_id)"
        "    );"
        "CREATE TABLE IF NOT EXISTS messages ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    sender_id INTEGER NOT NULL,"
        "    receiver_id INTEGER NOT NULL,"
        "    is_receiver_user BOOLEAN DEFAULT 1,"
        "    content TEXT NOT NULL,"
        "    audioURL TEXT,"
        "    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "    type BOOLEAN DEFAULT 0," // 0: text, 1: audio
        "    FOREIGN KEY (sender_id) REFERENCES USERS(id)"
        "    );";
    char* errMsg = nullptr;
    sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg);
}
