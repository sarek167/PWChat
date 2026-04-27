#ifndef LOGOUTCOMMAND_H
#define LOGOUTCOMMAND_H

#include "Command.h"

class LogoutCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};


#endif // LOGOUTCOMMAND_H
