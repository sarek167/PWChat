#include "server/Commands/LogoutCommand.h"
#include "server/Server.h"
#include "common/RoomData.h"

void LogoutCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    server.removeClient(session);

    std::vector<RoomData> userRooms = server.db().getUserRooms(p.header().senderId);
    std::cout << "Found " << userRooms.size() << " rooms for user " << p.header().senderId << std::endl;
    server.roomManager().logoutInitialize(userRooms, session);

    session->logout();

    Packet responsePacket(MessageType::LOGOUT_REQUEST, p.header().senderId, 0, NULL);
    session->deliver(responsePacket);
}
