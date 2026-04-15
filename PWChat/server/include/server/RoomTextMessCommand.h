#ifndef ROOMTEXTMESSCOMMAND_H
#define ROOMTEXTMESSCOMMAND_H

#include "Command.h"

class RoomTextMessCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // ROOMTEXTMESSCOMMAND_H
