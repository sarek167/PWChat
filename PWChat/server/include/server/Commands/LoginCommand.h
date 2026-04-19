#ifndef LOGINCOMMAND_H
#define LOGINCOMMAND_H

#include "Command.h"

class LoginCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // LOGINCOMMAND_H
