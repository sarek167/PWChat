#ifndef LOGINCOMMAND_H
#define LOGINCOMMAND_H

#include "Command.h"

/**
 * @class LoginCommand
 * @brief Command handler used when a user tries to log into the application.
 * * It inherits from the base Command class and overrides the execute method
 * to handle verifying user credentials.
 */
class LoginCommand : public Command {
public:
    /**
     * @brief Executes the user login logic.
     * Extracts the nickname and password from the packet, checks them against the database,
     * marks the session as authenticated if they match, and loads the user's initial room list.
     * @param session Shared pointer to the network session of the client trying to log in.
     * @param p The incoming Packet containing the entered nickname and password.
     * @param server Reference to the main Server instance to access the database and managers.
     */
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // LOGINCOMMAND_H
