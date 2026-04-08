#ifndef JOINROOMCOMMAND_H
#define JOINROOMCOMMAND_H

#include "Command.h"

class JoinRoomCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p) override;
};

#endif // JOINROOMCOMMAND_H
