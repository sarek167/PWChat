#include "server/Commands/UserTextMessCommand.h"
#include "server/Server.h"

void UserTextMessCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    uint32_t targetId = p.header().targetId;
    const std::shared_ptr<Session> targetClient = server.client(targetId);
    targetClient->deliver(p);
}
