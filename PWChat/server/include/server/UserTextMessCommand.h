#ifndef TEXTMESSCOMMAND_H
#define TEXTMESSCOMMAND_H

#include "Command.h"

class UserTextMessCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // TEXTMESSCOMMAND_H
