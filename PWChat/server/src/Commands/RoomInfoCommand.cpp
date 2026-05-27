#include "server/Commands/RoomInfoCommand.h"
#include "server/Server.h"
#include "common/RoomUserData.h"
#include "server/PrivateRoom.h"

void RoomInfoCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    uint32_t roomId = p.unpackBody<uint32_t>();;
    std::shared_ptr<Room> room = server.roomManager().getRoom(roomId);

    std::vector<UserData> users = server.db().getRoomUsers(room->id());
    std::vector<UserData> admins = server.db().getRoomUsers(room->id(), true);

    RoomUserData roomUserData;
    roomUserData.id = room->id();
    roomUserData.name = room->name();
    if (auto privateRoom = std::dynamic_pointer_cast<PrivateRoom>(room)) {
        roomUserData.isPrivate = true;
        roomUserData.accessCode = server.db().getRoomCode(room->id());
    } else {
        roomUserData.isPrivate = false;
    }
    roomUserData.users = users;
    roomUserData.admins = admins;

    std::cout << "Access code" << roomUserData.accessCode << std::endl;

    Packet responsePacket(MessageType::ROOM_INFO_REQUEST, p.header().senderId, 0, roomUserData);
    session->deliver(responsePacket);
}
