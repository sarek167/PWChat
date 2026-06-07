#ifndef LEAVEROOMCOMMAND_H
#define LEAVEROOMCOMMAND_H

#include "Command.h"

/**
 * @class LeaveRoomCommand
 * @brief Command handler used when a user wants to leave a chat room.
 * * It inherits from the base Command class and overrides the execute method
 * to handle removing a user from a specific room's active member list.
 */
class LeaveRoomCommand : public Command {
public:
    /**
     * @brief Executes the room leaving logic.
     * Extracts the room ID from the packet, removes the user's session from the room,
     * updates the database to reflect that they left, and notifies the remaining members.
     * @param session Shared pointer to the network session of the user who wants to leave the room.
     * @param p The incoming Packet containing the target room ID.
     * @param server Reference to the main Server instance to access the database and room manager.
     */
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // LEAVEROOMCOMMAND_H
