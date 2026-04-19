#include "server/Commands/RoomTextMessCommand.h"
#include "server/Server.h"

void RoomTextMessCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    try {
        server.roomManager().getRoom(p.header().targetId)->broadcast(p);
    } catch (...) {
        std::cerr << "Error while sending message to room" << std::endl;
    }
}
