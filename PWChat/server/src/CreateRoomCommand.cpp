#include "server/CreateRoomCommand.h"
#include "common/CreateRoomRequest.h"
#include "server/Server.h"

void CreateRoomCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    CreateRoomRequest request = p.unpackBody<CreateRoomRequest>();
    server.roomManager().createRoom(request.roomName, request.isPrivate, session->user(), request.isAdmin);
    server.roomManager().getRoom(request.roomName)->addClient(session);
}
