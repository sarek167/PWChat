#include "server/Commands/RoomMessCommand.h"
#include "server/Server.h"
#include "common/MessageData.h"
#include <fstream>
#include <ctime>
#include <filesystem>


void RoomMessCommand::execute(std::shared_ptr<Session> session, const Packet& p, Server& server) {
    try {
        uint32_t targetId = p.header().targetId;
        uint32_t senderId = p.header().senderId;

        MessageData message = p.unpackBody<MessageData>();

        std::string dbContent = message.message;

        if (message.messageType == MessageContentType::AUDIO) {
            std::string dirPath = "server_storage/audio/";
            std::string fileName = "voice_" + std::to_string(senderId) + "_" + std::to_string(std::time(nullptr)) + ".opus";
            std::string fullPath = dirPath + fileName;

            std::filesystem::create_directories(dirPath);

            std::ofstream file(fullPath, std::ios::out | std::ios::binary);
            if (file.is_open()) {
                file.write(message.message.data(), message.message.size());
                file.close();

                dbContent = fileName;
                message.message = fileName;
            } else {
                std::cerr << "Error Folder 'server_storage/audio/' does not exist or there are no sufficient permissions" << std::endl;
                Packet returnPacket(MessageType::ERROR_RESPONSE, senderId, 0, "Server file system error");
                session->deliver(returnPacket);
                return;
            }
        }
        bool dbResult = server.db().saveMessage(p.header().senderId, targetId, dbContent, message.messageType, true);
        if (dbResult) {
            Packet returnPacket(MessageType::MESS_TO_ROOM, p.header().senderId, 0, message);
            server.roomManager().getRoom(targetId)->broadcast(returnPacket);
        } else {

            Packet returnPacket(MessageType::ERROR_RESPONSE, p.header().senderId, 0, "Error while sending message");
            session->deliver(returnPacket);
        }
    } catch (...) {
        std::cerr << "Error while sending message to room" << std::endl;
    }
}
