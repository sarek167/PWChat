#include "server/Server.h"
#include "server/Session.h"
#include <iostream>

using asio::ip::tcp;

Server::Server(asio::io_context& io_context, short port)
        : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

const std::shared_ptr<Session> Server::client(uint32_t clientId) {
    return m_clients[clientId];
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
        targetClient->deliver(p);
    } else {
        std::cout << "Message was not to user - right now it's the only option :c" << std::endl;
    }

}
