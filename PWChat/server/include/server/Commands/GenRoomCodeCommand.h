#ifndef GENROOMCODECOMMAND_H
#define GENROOMCODECOMMAND_H


#include "Command.h"

class GenRoomCodeCommand : public Command {
public:
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // GENROOMCODECOMMAND_H
