#ifndef FINDUSERCOMMAND_H
#define FINDUSERCOMMAND_H

#include "Command.h"

class FindUserCommand : public Command {
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};


#endif // FINDUSERCOMMAND_H
