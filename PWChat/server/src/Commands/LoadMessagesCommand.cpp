#include "server/Commands/LoadMessagesCommand.h"
#include "server/Server.h"
#include "common/MessageRequest.h"

void LoadMessagesCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    MessageRequest req = p.unpackBody<MessageRequest>();
    try {
        std::vector<MessageData> messages = server.db().getMessages(req.targetId, session->userId(), req.fromRoom, 10, req.offset);
        Packet confirmationPacket(MessageType::LOAD_MESS_REQUEST, p.header().senderId, 0, messages);
        session->deliver(confirmationPacket);
    } catch (...) {
        Packet errorPacket(MessageType::ERROR_RESPONSE, p.header().senderId, 0, "Error while loading messages from database");
        session->deliver(errorPacket);
    }

}
