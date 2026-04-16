#include "server/CreateRoomCommand.h"
#include "common/CreateRoomRequest.h"
#include "server/Server.h"

void CreateRoomCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    CreateRoomRequest request = p.unpackBody<CreateRoomRequest>();

    uint32_t roomId = server.db().saveRoom(request.roomName, request.isPrivate, p.header().senderId);
    std::cout << "Saving room: " << roomId << std::endl;

    if (roomId) {
        server.roomManager().createRoom(roomId, request.roomName, request.isPrivate, p.header().senderId, request.isAdmin);
        server.roomManager().getRoom(request.roomName)->addClient(session);
    } else {
        std::cerr << "Error while saving room to db" << std::endl;
    }
}
