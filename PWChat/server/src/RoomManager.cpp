#include "server/RoomManager.h"
#include "server/PrivateRoom.h"
#include "server/PublicRoom.h"

std::map<uint32_t, std::shared_ptr<Room>> RoomManager::allRooms() {
    return m_allRooms;
}


std::shared_ptr<Room> RoomManager::getRoom(uint32_t id) {
    return m_allRooms[id];
}

std::shared_ptr<Room> RoomManager::getRoom(std::string name) {
    return m_allRoomsByName[name];
}


std::shared_ptr<Room> RoomManager::createRoom(uint32_t roomId, std::string name, bool isPrivate, uint32_t ownerId) {
    std::shared_ptr<Room> newRoom;
    if (isPrivate) {
        newRoom = std::make_shared<PrivateRoom>(roomId, name, ownerId);
    } else {
        newRoom = std::make_shared<PublicRoom>(roomId, name, ownerId);
    }

    m_allRooms[roomId] = newRoom;
    m_allRoomsByName[name] = newRoom;
    std::cout << "Created room with id " << roomId << std::endl;
    return newRoom;
}

void RoomManager::initialize(const std::vector<RoomData>& rooms) {
    m_allRooms.clear();
    m_allRoomsByName.clear();

    for (const auto& rd : rooms) {
        createRoom(rd.id, rd.name, rd.isPrivate, rd.ownerId);
    }
}

void RoomManager::loginInitialize(const std::vector<RoomData>& rooms, const std::shared_ptr<Session> session) {
    for (auto& rd : rooms) {
        std::shared_ptr<Room> room = getRoom(rd.id);
        room->addClient(session);
    }
}

// TO DO: add remove room
