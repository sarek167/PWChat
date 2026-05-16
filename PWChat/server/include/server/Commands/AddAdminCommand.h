#ifndef ADDADMINCOMMAND_H
#define ADDADMINCOMMAND_H

#include "Command.h"

class AddAdminCommand : public Command {
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};


#endif // ADDADMINCOMMAND_H
