#include "server/Commands/GenRoomCodeCommand.h"
#include "server/Server.h"

void GenRoomCodeCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    uint32_t roomId = p.unpackBody<uint32_t>();

    uint32_t code = 10000 + (rand() % (99999 - 10000 + 1));

    bool dbResult = server.db().saveRoomCode(roomId, code);

    if (dbResult) {
        Packet returnPacket(MessageType::GEN_CODE_REQUEST, p.header().senderId, 0, code);
        session->deliver(returnPacket);
    } else {
        Packet returnPacket(MessageType::ERROR_RESPONSE, p.header().senderId, 0, "Error while generating access code to room");
        session->deliver(returnPacket);
    }
}
