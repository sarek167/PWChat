#ifndef COMMAND_H
#define COMMAND_H

#include "server/Session.h"
#include "common/Packet.h"

/**
 * @class Command
 * @brief Base interface for the Command pattern used to handle incoming packets.
 * * Every specific action (like logging in, joining a room, or sending a message)
 * inherits from this class and implements its own execute method.
 */
class Command {
public:
    /**
     * @brief Virtual destructor to ensure proper cleanup of derived command objects.
     */
    virtual ~Command() = default;

    /**
     * @brief Pure virtual method that executes the logic for a specific packet type.
     * Must be implemented by all derived command classes.
     * @param session Shared pointer to the session of the client who sent the request.
     * @param p The incoming Packet data.
     * @param server Reference to the main Server instance to access subsystems.
     */
    virtual void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) = 0;
};

#endif // COMMAND_H
