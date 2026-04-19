#include "server/Commands/JoinRoomCommand.h"
#include "server/Room.h"
#include "server/PrivateRoom.h"
#include "server/Server.h"

void JoinRoomCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    std::string roomName = p.unpackBody<std::string>();
    std::shared_ptr<Room> room = server.roomManager().getRoom(roomName);

    bool dbResult = server.db().saveUserRoom(p.header().senderId, room->id(), false);

    if (dbResult) {
        room->addClient(session);
        std::cout << "Adding user to room" << std::endl;
        RoomData roomData;
        roomData.id = room->id();
        roomData.name = roomName;
        if (auto privateRoom = std::dynamic_pointer_cast<PrivateRoom>(room)) {
            roomData.isPrivate = true;
        } else {
            roomData.isPrivate = false;
        }
        roomData.ownerId = room->ownerId();
        Packet confirmationPacket(MessageType::CREATE_ROOM_COMM, p.header().senderId, 0, roomData);
        session->deliver(confirmationPacket);
    } else {
        std::cerr << "Error while adding user room" << std::endl;
    }

}
