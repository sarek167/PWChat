#include "server/JoinRoomCommand.h"
#include "server/Room.h"
#include "server/Server.h"

void JoinRoomCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    std::string roomName = p.unpackBody<std::string>();
    std::shared_ptr<Room> room = server.roomManager().getRoom(roomName);

    bool dbResult = server.db().saveUserRoom(p.header().senderId, room->id(), false);

    if (dbResult) {
        room->addClient(session);
        std::cout << "Adding user to room" << std::endl;
    } else {
        std::cerr << "Error while adding user room" << std::endl;
    }

}
