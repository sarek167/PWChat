#ifndef USERAUDIOMESSCOMMAND_H
#define USERAUDIOMESSCOMMAND_H

#include "Command.h"

class UserAudioMessCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // USERAUDIOMESSCOMMAND_H
