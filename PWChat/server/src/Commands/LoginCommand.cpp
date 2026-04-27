#include "server/Commands/LoginCommand.h"
#include "common/LoginRequest.h"
#include "server/Server.h"
#include "common/RoomData.h"

void LoginCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    LoginRequest loginReq = p.unpackBody<LoginRequest>();

    uint32_t userId = server.db().loginUser(loginReq.nickname, loginReq.password);

    if (userId == 0) {
        std::string errorMessage = "Wrong login or password...";
        Packet responsePacket(MessageType::ERROR_RESPONSE, 0, 0, errorMessage);
        session->deliver(responsePacket);
        return;
    }

    std::cout << "Logging in user " << userId << " with nickname " << loginReq.nickname << std::endl;
    session->setUser(userId, loginReq.nickname);
    server.insertClient(session);

    std::vector<RoomData> userRooms = server.db().getUserRooms(userId);
    std::cout << "Found " << userRooms.size() << " rooms for user " << userId << std::endl;
    server.roomManager().loginInitialize(userRooms, session);

    Packet responsePacket(MessageType::LOGIN_REQUEST, userId, 0, userRooms);
    session->deliver(responsePacket);

}
