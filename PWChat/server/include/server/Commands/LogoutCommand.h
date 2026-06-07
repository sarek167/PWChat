#ifndef LOGOUTCOMMAND_H
#define LOGOUTCOMMAND_H

#include "Command.h"

/**
 * @class LogoutCommand
 * @brief Command handler used when a user wants to log out of the server.
 * * It inherits from the base Command class and overrides the execute method
 * to handle clearing the user's active session state.
 */
class LogoutCommand : public Command {
public:
    /**
     * @brief Executes the user logout logic.
     * Clears the authentication flag for the session, unlinks the user from any active
     * rooms they were browsing, and sends a confirmation back before closing or resetting the state.
     * @param session Shared pointer to the network session of the client who wants to log out.
     * @param p The incoming Packet containing the logout request.
     * @param server Reference to the main Server instance to access managers and update active clients.
     */
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};


#endif // LOGOUTCOMMAND_H
