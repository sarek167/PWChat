#include "server/Commands/FindUserCommand.h"
#include "server/Server.h"

void FindUserCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    uint32_t senderId = p.header().senderId;
    std::string username = p.unpackBody<std::string>();
    uint32_t userId = server.db().findUserByNick(username);

    UserData foundUser;
    foundUser.id = userId;
    foundUser.nickname = username;
    if (userId) {
        Packet returnPacket(MessageType::FIND_USER_REQUEST, senderId, 0, foundUser);
        session->deliver(returnPacket);
    } else {
        Packet returnPacket(MessageType::ERROR_RESPONSE, senderId, 0, "User with given username could not be found");
        session->deliver(returnPacket);
    }
}
