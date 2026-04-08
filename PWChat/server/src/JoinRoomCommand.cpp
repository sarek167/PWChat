#include "server/JoinRoomCommand.h"

void JoinRoomCommand::execute(std::shared_ptr<Session> session, const Packet& p) {
    std::cout << "Adding user to room" << std::endl;
}
