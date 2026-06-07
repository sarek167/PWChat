#ifndef USERMESSCOMMAND_H
#define USERMESSCOMMAND_H

#include "Command.h"

/**
 * @class UserMessCommand
 * @brief Command handler used when a user sends a private, direct message to another user.
 * * It inherits from the base Command class and overrides the execute method
 * to handle private text or audio chat messages between two clients.
 */
class UserMessCommand : public Command {
public:
    /**
     * @brief Executes the private message processing logic.
     * Extracts the recipient's user ID and message content from the packet, saves the message
     * to the database history, and routes it directly to the recipient if they are currently online.
     * @param session Shared pointer to the network session of the user who sent the private message.
     * @param p The incoming Packet containing the target user ID and the message text or audio data.
     * @param server Reference to the main Server instance to access the database and client routing.
     */
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // USERMESSCOMMAND_H
