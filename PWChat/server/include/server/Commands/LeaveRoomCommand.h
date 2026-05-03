#ifndef LEAVEROOMCOMMAND_H
#define LEAVEROOMCOMMAND_H

#include "Command.h"

class LeaveRoomCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // LEAVEROOMCOMMAND_H
