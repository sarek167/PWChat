#include "server/Commands/CreateRoomCommand.h"
#include "common/CreateRoomRequest.h"
#include "server/Server.h"

void CreateRoomCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    CreateRoomRequest request = p.unpackBody<CreateRoomRequest>();

    uint32_t roomId = server.db().saveRoom(request.roomName, request.isPrivate, p.header().senderId);
    std::cout << "Saving room: " << roomId << std::endl;

    if (roomId) {
        std::shared_ptr<Room> addedRoom = server.roomManager().createRoom(roomId, request.roomName, request.isPrivate, p.header().senderId);

        if (request.isAdmin) {
            bool dbResult = server.db().saveUserRoom(p.header().senderId, addedRoom->id(), true);

            if (dbResult) {
                addedRoom->addClient(session);
                std::cout << "Adding user to room" << std::endl;
            } else {
                std::cerr << "Error while adding user room" << std::endl;
            }
            addedRoom->addAdmin(p.header().senderId);
        }
        RoomData roomData;
        roomData.id = roomId;
        roomData.name = request.roomName;
        roomData.isPrivate = request.isPrivate;
        roomData.ownerId = p.header().senderId;
        Packet confirmationPacket(MessageType::CREATE_ROOM_COMM, p.header().senderId, 0, roomData);
        session->deliver(confirmationPacket);
    } else {
        std::cerr << "Error while saving room to db" << std::endl;
    }
}
