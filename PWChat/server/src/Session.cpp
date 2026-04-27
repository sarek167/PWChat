#include "server/Session.h"
#include "server/Server.h"
#include <iostream>

using asio::ip::tcp;

Session::Session(tcp::socket socket, Server& server)
    : m_socket(std::move(socket)), m_server(server) {};

uint32_t Session::userId() {
    return m_user->id();
}

std::shared_ptr<User> Session::user() {
    return m_user;
}

void Session::setUser(uint32_t id, std::string nickname) {
    m_user = std::make_unique<User>(id, nickname);
    m_isAuthenticated = true;
}

bool Session::isAuthenticated() const {
    return m_isAuthenticated;
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
        if (!ec) {
            std::vector<char> rawBody(header.bodySize);
            auto bufs = m_buffer.data();
            std::copy(asio::buffers_begin(bufs), asio::buffers_begin(bufs) + header.bodySize, rawBody.begin());
            m_buffer.consume(header.bodySize);

            Packet packet(header, rawBody);
            m_server.onPacketReceived(self, packet);

            waitForRequest();
        }

    });
}

void Session::deliver(const Packet& p){
    std::vector<char> toSend = p.pack();

    auto self(shared_from_this());
    asio::async_write(m_socket, asio::buffer(toSend),
        [this, self, title = p.header().targetId, data = toSend](std::error_code ec, std::size_t bytesTransferred) {
            if (!ec) {
                std::cout << "Succesfully sent " << bytesTransferred << " bytes to User " << title << std::endl;
            } else {
                std::cerr << "Send error " << ec.message() << std::endl;
            }
    });
}
