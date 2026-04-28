#ifndef ROOMINFOCOMMAND_H
#define ROOMINFOCOMMAND_H

#include "Command.h"

class RoomInfoCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};


#endif // ROOMINFOCOMMAND_H
