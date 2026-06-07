#ifndef CREATEROOMCOMMAND_H
#define CREATEROOMCOMMAND_H

#include "Command.h"

/**
 * @class CreateRoomCommand
 * @brief Command handler used when a user requests to create a new chat room.
 * * It inherits from the base Command class and overrides the execute method
 * to handle setting up a new public or private room.
 */
class CreateRoomCommand : public Command {
public:
    /**
     * @brief Executes the room creation logic.
     * Extracts the new room details from the packet, saves the room in the database,
     * registers it within the server's room manager, and sends a confirmation back to the client.
     * @param session Shared pointer to the network session of the user who wants to create the room.
     * @param p The incoming Packet containing the desired room name and privacy settings.
     * @param server Reference to the main Server instance to access the database and room manager.
     */
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // CREATEROOMCOMMAND_H
