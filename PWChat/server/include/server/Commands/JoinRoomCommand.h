#ifndef JOINROOMCOMMAND_H
#define JOINROOMCOMMAND_H

#include "Command.h"

/**
 * @class JoinRoomCommand
 * @brief Command handler used when a user wants to join an existing chat room.
 * * It inherits from the base Command class and overrides the execute method
 * to handle entry requests for both public and private rooms.
 */
class JoinRoomCommand : public Command {
public:
    /**
     * @brief Executes the room joining logic.
     * Extracts the room ID and optional access code from the packet, checks if the room exists
     * and if the user is allowed to enter, updates the database/room state, and links the user session.
     * @param session Shared pointer to the network session of the user who wants to join the room.
     * @param p The incoming Packet containing the target room ID and password token (if private).
     * @param server Reference to the main Server instance to access the database and room manager.
     */
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // JOINROOMCOMMAND_H
