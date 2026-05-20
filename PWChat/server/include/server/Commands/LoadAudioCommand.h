#ifndef LOADAUDIOCOMMAND_H
#define LOADAUDIOCOMMAND_H

#include "Command.h"

class LoadAudioCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // LOADAUDIOCOMMAND_H
