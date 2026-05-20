#include "server/Commands/LoadAudioCommand.h"
#include "server/Server.h"
#include "common/MessageData.h"
#include <fstream>
#include <ctime>

void LoadAudioCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    uint32_t senderId = p.header().senderId;

    std::string fileName = p.unpackBody<std::string>();

    std::string fullPath = "server_storage/audio/" + fileName;


    std::ifstream file(fullPath, std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        std::streamsize fileSize = file.tellg();

        if (fileSize <= 0) {
            std::cerr << "File is empty or corrupted: " << fullPath << std::endl;
            file.close();
            Packet returnPacket(MessageType::ERROR_RESPONSE, senderId, 0, "Audio file corrupted");
            session->deliver(returnPacket);
            return;
        }

        file.seekg(0, std::ios::beg);
        std::vector<char> buffer(static_cast<size_t>(fileSize));

        if (file.read(buffer.data(), fileSize)) {
            file.close();

            MessageData audioMessage;
            audioMessage.senderId = senderId;
            audioMessage.targetId = 0;
            audioMessage.messageType = MessageContentType::AUDIO;
            audioMessage.message = std::string(buffer.data(), buffer.size());

            Packet responsePacket(MessageType::LOAD_AUDIO, senderId, 0, audioMessage);
            session->deliver(responsePacket);
            return;
        }

        file.close();

    } else {
        std::cerr << "Error while reading Audio from file" << std::endl;
        Packet returnPacket(MessageType::ERROR_RESPONSE, senderId, 0, "Server file system error");
        session->deliver(returnPacket);
        return;
    }

}
