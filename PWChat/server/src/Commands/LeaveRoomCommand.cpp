#include "server/Commands/LeaveRoomCommand.h"
#include "server/Room.h"
#include "server/Server.h"
#include "common/LeaveRoomRequest.h"

void LeaveRoomCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    LeaveRoomRequest req = p.unpackBody<LeaveRoomRequest>();
    std::shared_ptr<Room> room = server.roomManager().getRoom(req.roomId);

    if (req.userId != session->userId() && !room->checkIfAdmin(session->userId())) {
        std::cerr << "Error - only admins can remove users from rooms." << std::endl;
        return;
    }

    bool dbResult = server.db().deleteUserRoom(req.userId, req.roomId);

    if (dbResult) {
        room->removeClient(session);
        std::cout << "Removing user from room" << std::endl;
        Packet confirmationPacket(MessageType::LEAVE_ROOM_REQUEST, p.header().senderId, 0, req);
        session->deliver(confirmationPacket);
    } else {
        std::cerr << "Error while removing user from room" << std::endl;
    }

}
