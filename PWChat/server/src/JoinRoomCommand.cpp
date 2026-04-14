#include "server/JoinRoomCommand.h"
#include "server/Room.h"
#include "server/Server.h"

void JoinRoomCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    std::string roomName = p.unpackBody<std::string>();
    std::shared_ptr<Room> room = server.roomManager().getRoom(roomName);
    room->addClient(session);
    std::cout << "Adding user to room" << std::endl;
}
