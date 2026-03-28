#include "server/Session.h"
#include "server/Server.h"
#include <iostream>

using asio::ip::tcp;

Session::Session(tcp::socket socket, Server& server)
    : m_socket(std::move(socket)), m_server(server) {};

uint32_t Session::userId() {
    return m_user->id();
}

void Session::setUser(uint32_t id, std::string nickname) {
    m_user = std::make_unique<User>(id, nickname);
}

void Session::handleLogin(uint32_t id, std::string nickname) {
    this->setUser(id, nickname);
    m_server.insertClient(shared_from_this());
}

void Session::do_read() {
    wait_for_request();
}

void Session::wait_for_request() {
    auto self(shared_from_this());
    asio::async_read_until(m_socket, m_buffer, "\0",
                           [this,self](std::error_code ec, std::size_t) {
        if (!ec) {
            std::vector<char> rawData{
                std::istreambuf_iterator<char>(&m_buffer),
                std::istreambuf_iterator<char>()
            };
            PacketHeader header = Packet::unpackHeader(rawData);
            if (header.type == MessageType::LOGIN_REQUEST) {
                handleLogin(header.senderId, "Nick");
            } else if (header.type == MessageType::TEXT_TO_USER) {
                Packet receivedPacket = Packet::unpack(rawData);
                m_server.routePacket(receivedPacket);
                std::cout << receivedPacket.header().signature << std::endl;
                std::cout << (int)receivedPacket.header().type << std::endl;
                std::cout << (int)receivedPacket.header().bodySize << std::endl;
                std::string message{receivedPacket.body().begin(), receivedPacket.body().end()};
                std::cout << message << std::endl;

            } else {
                std::cout << "takiego typu jeszcze nie obsługujemy" << std::endl;
            }

            wait_for_request();
        } else {
            std::cout << "error: " << ec << std::endl;
        }
    });
}

void Session::deliver(const Packet& p) const {
    std::cout << "Got package! From: " << p.header().senderId << std:: endl;
    std::cout << "Will be delivered to: " << p.header().targetId << std::endl;
    std::cout << "User session: " << m_user->id() << std::endl;
}
