#include "server/Commands/AddAdminCommand.h"
#include "common/AddAdminRequest.h"
#include "server/Server.h"

void AddAdminCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    uint32_t senderId = p.header().senderId;
    AddAdminRequest req = p.unpackBody<AddAdminRequest>();
    bool dbResult = server.db().addAdmin(req.roomId, req.userId);

    if (dbResult) {
        Packet returnPacket(MessageType::ADD_ADMIN_REQUEST, senderId, 0, req);
        session->deliver(returnPacket);
    } else {
        Packet returnPacket(MessageType::ERROR_RESPONSE, senderId, 0, "Error while adding admin to the room");
        session->deliver(returnPacket);
    }
}
