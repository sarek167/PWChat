#include "server/Commands/JoinRoomCommand.h"
#include "server/Room.h"
#include "server/PrivateRoom.h"
#include "server/Server.h"
#include "common/JoinRoomRequest.h"

void JoinRoomCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    JoinRoomRequest joinReq = p.unpackBody<JoinRoomRequest>();
    std::shared_ptr<Room> room = server.roomManager().getRoom(joinReq.name);

    if (!room) {
        Packet errorPacket(MessageType::ERROR_RESPONSE, p.header().senderId, 0, "Requested room does not exist");
        session->deliver(errorPacket);
        return;
    }

    RoomData roomData;
    roomData.id = room->id();
    roomData.name = joinReq.name;

    if (auto privateRoom = std::dynamic_pointer_cast<PrivateRoom>(room)) {
        if (joinReq.token == 0) {
            Packet missingTokenPacket(MessageType::ACCESS_CODE_REQUIRED, p.header().senderId, 0, joinReq);
            session->deliver(missingTokenPacket);
            return;
        } else {
            uint32_t dbCode = server.db().getRoomCode(room->id());
            if (joinReq.token != dbCode) {
                Packet returnPacket(MessageType::ERROR_RESPONSE, p.header().senderId, 0, "Access code to the room is incorrect");
                session->deliver(returnPacket);
                return;
            }
        }
        roomData.isPrivate = true;
    } else {
        roomData.isPrivate = false;
    }
    roomData.ownerId = room->ownerId();

    bool dbResult = server.db().saveUserRoom(p.header().senderId, room->id(), false);
    if (dbResult) {
        room->addClient(session);
        std::cout << "Adding user to room" << std::endl;
        Packet confirmationPacket(MessageType::JOIN_ROOM_COMM, p.header().senderId, 0, roomData);
        session->deliver(confirmationPacket);
    } else {
        std::cerr << "Error while adding user room" << std::endl;
    }
}
