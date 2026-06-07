#ifndef FINDUSERCOMMAND_H
#define FINDUSERCOMMAND_H

#include "Command.h"

/**
 * @class FindUserCommand
 * @brief Command handler used when a user wants to search for another user by their nickname.
 * * It inherits from the base Command class and overrides the execute method
 * to handle looking up user profiles in the system.
 */
class FindUserCommand : public Command {
public:
    /**
     * @brief Executes the user search logic.
     * Extracts the target nickname from the packet, searches for it in the database,
     * and sends the search results (like the user's ID or status) back to the client.
     * @param session Shared pointer to the network session of the user who sent the search request.
     * @param p The incoming Packet containing the nickname string to look for.
     * @param server Reference to the main Server instance to access the database.
     */
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};


#endif // FINDUSERCOMMAND_H
