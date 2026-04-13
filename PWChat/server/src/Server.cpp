#include "server/Server.h"
#include "server/Session.h"
#include <iostream>
#include "server/LoginCommand.h"
#include "server/JoinRoomCommand.h"
#include "server/CreateRoomCommand.h"

using asio::ip::tcp;

Server::Server(asio::io_context& io_context, short port)
    : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port)), m_roomManager() {
        m_commands[MessageType::LOGIN_REQUEST] = std::make_unique<LoginCommand>();
        m_commands[MessageType::JOIN_ROOM_COMM] = std::make_unique<JoinRoomCommand>();
        m_commands[MessageType::CREATE_ROOM_COMM] = std::make_unique<CreateRoomCommand>();

        do_accept();
    }

void Server::onPacketReceived(std::shared_ptr<Session> session, const Packet& p) {
    MessageType messType = p.header().type;

    auto it = m_commands.find(messType);
    if (it != m_commands.end()) {
        it->second->execute(session, p, *this);
    } else {
        std::cout << "Packet does not contain Command -> routing it" << std::endl;
        routePacket(p);
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

void Server::routePacket(const Packet& p) {
    MessageType messType = p.header().type;
    uint32_t targetId = p.header().targetId;

    if (messType == MessageType::TEXT_TO_USER) {
        const std::shared_ptr<Session> targetClient = client(targetId);
        std::cout << "before adding to room" << std::endl;
        m_roomManager.getRoom("Lobby")->addClient(targetClient);
        targetClient->deliver(p);

    } else if (messType == MessageType::TEXT_TO_ROOM) {
        m_roomManager.getRoom("Lobby")->broadcast(p);
    } else {
        std::cout << "Message was not to user - right now it's the only option :c" << std::endl;
    }

}
