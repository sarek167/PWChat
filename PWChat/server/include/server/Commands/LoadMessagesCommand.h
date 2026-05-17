#ifndef LOADMESSAGESCOMMAND_H
#define LOADMESSAGESCOMMAND_H

#include "Command.h"

class LoadMessagesCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // LOADMESSAGESCOMMAND_H
