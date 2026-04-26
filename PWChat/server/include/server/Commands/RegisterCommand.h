#ifndef REGISTERCOMMAND_H
#define REGISTERCOMMAND_H

#include "Command.h"

class RegisterCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // REGISTERCOMMAND_H
