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

void Session::doRead() {
    waitForRequest();
}

void Session::waitForRequest() {
    auto self(shared_from_this());
    asio::async_read(m_socket, m_buffer, asio::transfer_exactly(sizeof(PacketHeader)),
                           [this,self](std::error_code ec, std::size_t bytesTransferred) {
        if (!ec) {
            PacketHeader header;
            asio::buffer_copy(asio::buffer(&header, sizeof(PacketHeader)), m_buffer.data());
            m_buffer.consume(bytesTransferred);


            readBody(header);

        } else {
            std::cout << "error: " << ec << std::endl;
        }
    });
}

void Session::readBody(PacketHeader header) {
    auto self(shared_from_this());
    asio::async_read(m_socket, m_buffer, asio::transfer_exactly(header.bodySize),
                           [this,self,header](std::error_code ec, std::size_t bytesTransferred) {
        std::istream is(&m_buffer);
        std::vector<char> deserializedBody;

        try {
            cereal::BinaryInputArchive iarchive(is);
            iarchive(deserializedBody);
        } catch (const std::exception& e) {
            std::cerr << "Cereal error: " << e.what() << std::endl;
        }

        Packet packet(header, deserializedBody);

        if (header.type == MessageType::LOGIN_REQUEST) {
            handleLogin(header.senderId, "Nick");
        } else {
            m_server.routePacket(packet);
            std::cout << packet.header().signature << std::endl;
            std::cout << (int)packet.header().type << std::endl;
            std::cout << (int)packet.header().bodySize << std::endl;
            std::string message{packet.body().begin(), packet.body().end()};
            std::cout << message << std::endl;

        }
        waitForRequest();
    });
}

void Session::deliver(const Packet& p) const {
    std::cout << "Got package! From: " << p.header().senderId << std:: endl;
    std::cout << "Will be delivered to: " << p.header().targetId << std::endl;
    std::cout << "User session: " << m_user->id() << std::endl;
}
