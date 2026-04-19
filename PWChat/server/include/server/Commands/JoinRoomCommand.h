#ifndef JOINROOMCOMMAND_H
#define JOINROOMCOMMAND_H

#include "Command.h"

class JoinRoomCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // JOINROOMCOMMAND_H
