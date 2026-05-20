#ifndef USERMESSCOMMAND_H
#define USERMESSCOMMAND_H

#include "Command.h"

class UserMessCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // USERMESSCOMMAND_H
