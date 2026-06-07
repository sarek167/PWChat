#ifndef ROOMINFOCOMMAND_H
#define ROOMINFOCOMMAND_H

#include "Command.h"

/**
 * @class RoomInfoCommand
 * @brief Command handler used when a client requests details about a specific chat room.
 * * It inherits from the base Command class and overrides the execute method
 * to handle fetching room metadata and member lists.
 */
class RoomInfoCommand : public Command {
public:
    /**
     * @brief Executes the room information retrieval logic.
     * Extracts the room ID from the packet, fetches the room details and lists of active members
     * or administrators from the database, and sends this data back to the requesting client.
     * @param session Shared pointer to the network session of the user who requested the room info.
     * @param p The incoming Packet containing the target room ID.
     * @param server Reference to the main Server instance to access the database and room manager.
     */
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};


#endif // ROOMINFOCOMMAND_H
