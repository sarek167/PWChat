#ifndef GENROOMCODECOMMAND_H
#define GENROOMCODECOMMAND_H


#include "Command.h"

/**
 * @class GenRoomCodeCommand
 * @brief Command handler used when an admin or owner wants to generate a new entry code for a private room.
 * * It inherits from the base Command class and overrides the execute method
 * to handle updating the room's access pin.
 */
class GenRoomCodeCommand : public Command {
public:
    /**
     * @brief Executes the room code generation logic.
     * Extracts the room ID from the packet, checks if the sender has permission to modify the room,
     * generates a new random access code, saves it in the database, and sends it back to the client.
     * @param session Shared pointer to the network session of the user who requested the new code.
     * @param p The incoming Packet containing the target room ID.
     * @param server Reference to the main Server instance to access the database and room manager.
     */
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // GENROOMCODECOMMAND_H
