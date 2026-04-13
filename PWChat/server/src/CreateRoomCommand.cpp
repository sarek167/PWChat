#include "server/CreateRoomCommand.h"
#include "common/RoomRequest.h"

void CreateRoomCommand::execute(std::shared_ptr<Session> session, const Packet& p) {
    RoomRequest request = p.unpackBody<RoomRequest>();
    std::cout << "Creating room" << std::endl;
    std::cout << request.roomName <<std::endl;
}
