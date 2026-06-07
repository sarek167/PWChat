#ifndef ADDADMINCOMMAND_H
#define ADDADMINCOMMAND_H

#include "Command.h"

/**
 * @class AddAdminCommand
 * @brief Command handler used when a room owner wants to promote another user to admin.
 * * It inherits from the base Command class and overrides the execute method
 * to handle this specific network request.
 */
class AddAdminCommand : public Command {
public:
    /**
     * @brief Executes the "add admin" logic.
     * Extracts details from the packet, verifies if the sender has permission
     * to promote others, updates the database, and notifies the room members.
     * @param session Shared pointer to the network session of the user who sent the request.
     * @param p The incoming Packet containing the target room ID and user ID to promote.
     * @param server Reference to the main Server instance to access the database and room manager.
     */
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};


#endif // ADDADMINCOMMAND_H
