#include "server/Commands/GenRoomCodeCommand.h"
#include "server/Server.h"
#include <random>

void GenRoomCodeCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    uint32_t roomId = p.unpackBody<uint32_t>();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dist(10000, 99999);
    uint32_t code = dist(gen);

    bool dbResult = server.db().saveRoomCode(roomId, code);

    if (dbResult) {
        Packet returnPacket(MessageType::GEN_CODE_REQUEST, p.header().senderId, 0, code);
        session->deliver(returnPacket);
    } else {
        Packet returnPacket(MessageType::ERROR_RESPONSE, p.header().senderId, 0, "Error while generating access code to room");
        session->deliver(returnPacket);
    }
}
