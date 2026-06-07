#ifndef REGISTERCOMMAND_H
#define REGISTERCOMMAND_H

#include "Command.h"

/**
 * @class RegisterCommand
 * @brief Command handler used when a new user wants to create an account.
 * * It inherits from the base Command class and overrides the execute method
 * to handle new user registration.
 */
class RegisterCommand : public Command {
public:
    /**
     * @brief Executes the user registration logic.
     * Extracts the new nickname and password from the packet, checks if the username
     * is already taken, saves the new user credentials into the database, and returns the result.
     * @param session Shared pointer to the network session of the client trying to register.
     * @param p The incoming Packet containing the desired nickname and password.
     * @param server Reference to the main Server instance to access the database.
     */
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // REGISTERCOMMAND_H
