#ifndef LOADAUDIOCOMMAND_H
#define LOADAUDIOCOMMAND_H

#include "Command.h"

/**
 * @class LoadAudioCommand
 * @brief Command handler used when a client requests to download or load a recorded audio message.
 * * It inherits from the base Command class and overrides the execute method
 * to handle retrieving audio data paths or binaries from the system.
 */
class LoadAudioCommand : public Command {
public:
    /**
     * @brief Executes the audio loading logic.
     * Extracts the message or file identifier from the packet, checks permissions,
     * pulls the corresponding audio data from the database or storage, and sends it back to the client.
     * @param session Shared pointer to the network session of the user who requested the audio.
     * @param p The incoming Packet containing the target audio or message ID.
     * @param server Reference to the main Server instance to access the database.
     */
    void execute(std::shared_ptr<Session> session, const Packet& p, Server& server) override;
};

#endif // LOADAUDIOCOMMAND_H
