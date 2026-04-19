#ifndef COMMAND_H
#define COMMAND_H

#include "server/Session.h"
#include "common/Packet.h"

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) = 0;
};

#endif // COMMAND_H
