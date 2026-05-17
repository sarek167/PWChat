#include "server/Commands/RoomTextMessCommand.h"
#include "server/Server.h"

void RoomTextMessCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    try {
        uint32_t targetId = p.header().targetId;
        std::string message = p.unpackBody<std::string>();
        bool dbResult = server.db().saveTextMessage(p.header().senderId, targetId, message, true);
        if (dbResult) {
            server.roomManager().getRoom(targetId)->broadcast(p);
        } else {
            Packet returnPacket(MessageType::ERROR_RESPONSE, p.header().senderId, 0, "Error while sending text message");
            session->deliver(returnPacket);
        }
    } catch (...) {
        std::cerr << "Error while sending message to room" << std::endl;
    }
}
