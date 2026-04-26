#include "server/Commands/RegisterCommand.h"
#include "common/RegisterRequest.h"
#include "server/Server.h"

void RegisterCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    RegisterRequest registerReq = p.unpackBody<RegisterRequest>();

    std::cout << "Registering user " << registerReq.id << " with nickname " << registerReq.nickname << std::endl;
    uint32_t id = server.db().registerUser(registerReq.nickname, registerReq.password);

    if (!id) {
        std::cerr << "Error while registering user with name " << registerReq.nickname << std::endl;
        return;
    }


    session->setUser(id, registerReq.nickname);
    server.insertClient(session);

    registerReq.id = id;
    registerReq.password = "";

    Packet responsePacket(MessageType::REGISTER_REQUEST, id, 0, registerReq);
    session->deliver(responsePacket);

}
