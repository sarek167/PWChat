#include "server/Server.h"
#include "server/Session.h"
#include <iostream>
#include "server/LoginCommand.h"
#include "server/JoinRoomCommand.h"
#include "server/CreateRoomCommand.h"
#include "server/RoomTextMessCommand.h"
#include "server/UserTextMessCommand.h"

using asio::ip::tcp;

Server::Server(asio::io_context& io_context, short port)
    : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port)), m_roomManager() {
        m_commands[MessageType::LOGIN_REQUEST] = std::make_unique<LoginCommand>();
        m_commands[MessageType::JOIN_ROOM_COMM] = std::make_unique<JoinRoomCommand>();
        m_commands[MessageType::CREATE_ROOM_COMM] = std::make_unique<CreateRoomCommand>();
        m_commands[MessageType::TEXT_TO_USER] = std::make_unique<UserTextMessCommand>();
        m_commands[MessageType::TEXT_TO_ROOM] = std::make_unique<RoomTextMessCommand>();

        do_accept();
    }

void Server::onPacketReceived(std::shared_ptr<Session> session, const Packet& p) {
    MessageType messType = p.header().type;

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
