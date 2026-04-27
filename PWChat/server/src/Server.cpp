#include "server/Server.h"
#include "server/Session.h"
#include <iostream>
#include "server/Commands/LoginCommand.h"
#include "server/Commands/JoinRoomCommand.h"
#include "server/Commands/RegisterCommand.h"
#include "server/Commands/CreateRoomCommand.h"
#include "server/Commands/RoomTextMessCommand.h"
#include "server/Commands/UserTextMessCommand.h"
#include "server/Commands/UserAudioMessCommand.h"
#include "server/SQLiteConnector.h"
using asio::ip::tcp;

Server::Server(asio::io_context& io_context, short port)
    : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port)), m_roomManager() {
        m_db = std::make_unique<SQLiteConnector>();

        if (m_db->connect("pwchat_db.db")) {
            static_cast<SQLiteConnector*>(m_db.get())->initializeSchema();
            std::cout << "DB is up and ready";
        } else {
            std::cerr << "Error while connecting to db" << std::endl;
        }

        loadDataFromDB();

        m_commands[MessageType::LOGIN_REQUEST] = std::make_unique<LoginCommand>();
        m_commands[MessageType::REGISTER_REQUEST] = std::make_unique<RegisterCommand>();
        m_commands[MessageType::JOIN_ROOM_COMM] = std::make_unique<JoinRoomCommand>();
        m_commands[MessageType::CREATE_ROOM_COMM] = std::make_unique<CreateRoomCommand>();
        m_commands[MessageType::TEXT_TO_USER] = std::make_unique<UserTextMessCommand>();
        m_commands[MessageType::TEXT_TO_ROOM] = std::make_unique<RoomTextMessCommand>();
        m_commands[MessageType::AUDIO_TO_USER] = std::make_unique<UserAudioMessCommand>();

        do_accept();
    }

void Server::onPacketReceived(std::shared_ptr<Session> session, const Packet& p) {
    MessageType messType = p.header().type;

    if ((messType != MessageType::LOGIN_REQUEST && messType != MessageType::REGISTER_REQUEST) && !session->isAuthenticated()) {
        std::string errorMessage = "The session is not authenticated - login first";
        Packet errorPacket = Packet(MessageType::ERROR_RESPONSE, 0, 0, errorMessage);
        session->deliver(errorPacket);
        return;
    }

    auto it = m_commands.find(messType);
    if (it != m_commands.end()) {
        it->second->execute(session, p, *this);
    } else {
        std::cerr << "Unknown message type received" << std::endl;
    }

}

RoomManager& Server::roomManager() {
    return m_roomManager;
}

DBConnector& Server::db() {
    return *m_db;
}

const std::shared_ptr<Session> Server::client(uint32_t clientId) {
    auto it = m_clients.find(clientId);

    if (it != m_clients.end()) {
        return it->second;
    }
    return nullptr;
}

void Server::do_accept() {
    m_acceptor.async_accept([this](asio::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::cout << "creatin session on: "
                      << socket.remote_endpoint().address().to_string()
                      << ":" << socket.remote_endpoint().port() << "\n";
            auto newSession = std::make_shared<Session>(std::move(socket), *this);
            newSession -> doRead();
        } else {
            std::cout << "error: " << ec.message() << std::endl;
        }

        do_accept();
    });
}

void Server::insertClient(std::shared_ptr<Session> session) {
    std::lock_guard<std::mutex> lock(m_clientsMutex);
    m_clients.insert({session->userId(), session});
}

void Server::loadDataFromDB() {
    auto roomsData = m_db->getAllRooms();
    m_roomManager.initialize(roomsData);
    std::cout << "Loaded " << roomsData.size() << "rooms from db" << std::endl;
}
