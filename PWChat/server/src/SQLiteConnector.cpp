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
        "    is_private BOOLEAN DEFAULT 0 NOT NULL," // 0: public, 1: private
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

int SQLiteConnector::saveRoom(const std::string& name, const bool isPrivate, const uint32_t ownerId) {
    const char* sql = "INSERT INTO rooms (name, is_private, owner_id) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL Error: " << sqlite3_errmsg(m_db) << std::endl;
        return 0;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, isPrivate ? 1 : 0);
    sqlite3_bind_int(stmt, 3, static_cast<int>(ownerId));

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        uint32_t newId = static_cast<uint32_t>(sqlite3_last_insert_rowid(m_db));
        sqlite3_finalize(stmt);
        return newId;
    }

    sqlite3_finalize(stmt);
    return 0;
}

std::vector<RoomData> SQLiteConnector::getAllRooms() {
    std::vector<RoomData> rooms;
    const char* sql = "SELECT id, name, is_private, owner_id FROM rooms;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            RoomData room;
            room.id = static_cast<uint32_t>(sqlite3_column_int(stmt, 0));
            room.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            room.isPrivate = sqlite3_column_int(stmt, 2) != 0;
            room.ownerId = static_cast<uint32_t>(sqlite3_column_int(stmt, 3));
            rooms.push_back(room);
        }
    }

    sqlite3_finalize(stmt);
    return rooms;
}

std::vector<RoomData> SQLiteConnector::getUserRooms(const uint32_t userId) {
    std::vector<RoomData> rooms;
    const char* sql =
        "SELECT r.id, r.name, r.is_private, r.owner_id "
        "FROM rooms r "
        "JOIN users_rooms ur ON r.id = ur.room_id "
        "WHERE ur.user_id = ?";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, static_cast<int>(userId));

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            RoomData room;
            room.id = static_cast<uint32_t>(sqlite3_column_int(stmt, 0));
            room.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            room.isPrivate = sqlite3_column_int(stmt, 2) != 0;
            room.ownerId = static_cast<uint32_t>(sqlite3_column_int(stmt, 3));
            rooms.push_back(room);
        }
    } else {
        std::cerr << "SQL Error: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return rooms;
}

bool SQLiteConnector::saveUserRoom(const uint32_t userId, const uint32_t roomId, bool isAdmin) {
    const char* sql = "INSERT INTO users_rooms (user_id, room_id, is_admin) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL Error: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, static_cast<int>(userId));
    sqlite3_bind_int(stmt, 2, static_cast<int>(roomId));
    sqlite3_bind_int(stmt, 3, isAdmin ? 1 : 0);

    int rc = sqlite3_step(stmt);
    bool success = (rc == SQLITE_DONE);

    if (!success) {
        std::cerr << "SQL Error (Step): " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return true;

}

