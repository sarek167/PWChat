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


void RoomManager::createRoom(std::string name, bool isPrivate, std::shared_ptr<User> owner, bool ownerIsAdmin) {
    std::shared_ptr<Room> newRoom;
    uint32_t newId = m_nextId++;
    if (isPrivate) {
        newRoom = std::make_shared<PrivateRoom>(newId, name);
    } else {
        newRoom = std::make_shared<PublicRoom>(newId, name);
    }

    if (ownerIsAdmin) {
        newRoom->addAdmin(owner);
    }
    m_allRooms[newId] = newRoom;
    m_allRoomsByName[name] = newRoom;
    std::cout << "Created room with id " << newId << std::endl;
}

// TO DO: add remove room
