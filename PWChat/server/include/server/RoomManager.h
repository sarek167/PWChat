#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include <string>
#include <memory>
#include <map>
#include "Room.h"
#include "RoomData.h"

class RoomManager {
public:
    std::map<uint32_t, std::shared_ptr<Room>> allRooms();
    std::shared_ptr<Room> getRoom(uint32_t id);
    std::shared_ptr<Room> getRoom(std::string name);
    std::shared_ptr<Room> createRoom(uint32_t roomId, std::string name, bool isPrivate, uint32_t ownerId);
    void removeRoom(std::string name);
    void initialize(const std::vector<RoomData>& rooms);
    void loginInitialize(const std::vector<RoomData>& rooms, const std::shared_ptr<Session> session);

private:
    std::map<uint32_t, std::shared_ptr<Room>> m_allRooms;
    std::map<std::string, std::shared_ptr<Room>> m_allRoomsByName;
};

#endif // ROOMMANAGER_H
