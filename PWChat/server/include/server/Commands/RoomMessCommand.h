#ifndef ROOMMESSCOMMAND_H
#define ROOMMESSCOMMAND_H

#include "Command.h"

class RoomMessCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // ROOMMESSCOMMAND_H
