#include "server/Commands/UserTextMessCommand.h"
#include "server/Server.h"

void UserTextMessCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    uint32_t targetId = p.header().targetId;
    std::string message = p.unpackBody<std::string>();
    bool dbResult = server.db().saveTextMessage(p.header().senderId, targetId, message, false);
    if (dbResult) {
        const std::shared_ptr<Session> targetClient = server.client(targetId);
        targetClient->deliver(p);
    } else {
        Packet returnPacket(MessageType::ERROR_RESPONSE, p.header().senderId, 0, "Error while sending text message");
        session->deliver(returnPacket);
    }

}
