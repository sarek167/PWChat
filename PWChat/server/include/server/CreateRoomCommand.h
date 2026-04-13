#ifndef CREATEROOMCOMMAND_H
#define CREATEROOMCOMMAND_H

#include "Command.h"

class CreateRoomCommand : public Command {
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // CREATEROOMCOMMAND_H
