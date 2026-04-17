#include "server/LoginCommand.h"
#include "common/LoginRequest.h"
#include "server/Server.h"
#include "server/RoomData.h"

void LoginCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    LoginRequest loginReq = p.unpackBody<LoginRequest>();

    std::cout << "Logging in user " << loginReq.userId << " with nickname " << loginReq.nickname << std::endl;
    session->setUser(loginReq.userId, loginReq.nickname);
    server.insertClient(session);

    std::vector<RoomData> userRooms = server.db().getUserRooms(p.header().senderId);
    std::cout << "Found " << userRooms.size() << " rooms for user " << p.header().senderId << std::endl;
    server.roomManager().loginInitialize(userRooms, session);

    std::string status{"success"};
    Packet responsePacket(MessageType::AUTH_RESPONSE, loginReq.userId, 0, status);
    session->deliver(responsePacket);

}
