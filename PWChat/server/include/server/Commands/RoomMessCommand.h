#ifndef ROOMMESSCOMMAND_H
#define ROOMMESSCOMMAND_H

#include "Command.h"

/**
 * @class RoomMessCommand
 * @brief Command handler used when a user sends a message to a chat room.
 * * It inherits from the base Command class and overrides the execute method
 * to handle broadcasting text or audio messages to an entire room.
 */
class RoomMessCommand : public Command {
public:
    /**
     * @brief Executes the room message processing logic.
     * Extracts the room ID and message contents from the packet, saves the message
     * to the database history, and broadcasts it to all other users currently inside that room.
     * @param session Shared pointer to the network session of the user who sent the message.
     * @param p The incoming Packet containing the target room ID and the message text or audio data.
     * @param server Reference to the main Server instance to access the database and room manager.
     */
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // ROOMMESSCOMMAND_H
