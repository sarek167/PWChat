#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include <string>
#include <memory>
#include <map>
#include "Room.h"

class RoomManager {
public:
    std::map<uint32_t, std::shared_ptr<Room>> allRooms();
    std::shared_ptr<Room> getRoom(uint32_t id);
    std::shared_ptr<Room> getRoom(std::string name);
    void createRoom(std::string name, bool isPrivate, std::shared_ptr<User> owner, bool ownerIsAdmin);
    void removeRoom(std::string name);

private:
    uint32_t m_nextId = 1000;
    std::map<uint32_t, std::shared_ptr<Room>> m_allRooms;
    std::map<std::string, std::shared_ptr<Room>> m_allRoomsByName;
};

#endif // ROOMMANAGER_H
