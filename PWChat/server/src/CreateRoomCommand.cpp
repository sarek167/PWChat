#include "server/CreateRoomCommand.h"

void CreateRoomCommand::execute(std::shared_ptr<Session> session, const Packet& p) {
    std::cout << "Creating room" << std::endl;
}
