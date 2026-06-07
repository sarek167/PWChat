#ifndef LOADMESSAGESCOMMAND_H
#define LOADMESSAGESCOMMAND_H

#include "Command.h"

/**
 * @class LoadMessagesCommand
 * @brief Command handler used when a client requests to load chat history.
 * * It inherits from the base Command class and overrides the execute method
 * to handle fetching archived messages for both rooms and private conversations.
 */
class LoadMessagesCommand : public Command {
public:
    /**
     * @brief Executes the message history loading logic.
     * Extracts parameters like target ID, limit, and offset from the packet,
     * pulls the matching batch of messages from the database, and sends them back to the client.
     * @param session Shared pointer to the network session of the user who requested the history.
     * @param p The incoming Packet containing pagination details and chat target identifiers.
     * @param server Reference to the main Server instance to access the database.
     */
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // LOADMESSAGESCOMMAND_H
