#include "server/RoomManager.h"
#include "server/PrivateRoom.h"
#include "server/PublicRoom.h"

std::map<std::string, std::shared_ptr<Room>> RoomManager::allRooms() {
    return m_allRooms;
}


std::shared_ptr<Room> RoomManager::getRoom(std::string name) {
    return m_allRooms[name];
}

void RoomManager::createRoom(std::string name, bool isPrivate, std::shared_ptr<User> owner, bool ownerIsAdmin) {
    std::shared_ptr<Room> newRoom;
    if (isPrivate) {
        newRoom = std::make_shared<PrivateRoom>(0, name);
    } else {
        newRoom = std::make_shared<PublicRoom>(0, name);
    }

    if (ownerIsAdmin) {
        newRoom->addAdmin(owner);
    }

    m_allRooms[name] = newRoom;
}

// TO DO: add remove room
