#include "server/UserAudioMessCommand.h"
#include "server/Server.h"

void UserAudioMessCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    std::cout << "Received audio package with size: " << p.header().bodySize << std::endl;
    uint32_t targetId = p.header().targetId;
    const std::shared_ptr<Session> targetClient = server.client(targetId);
    targetClient->deliver(p);
}
