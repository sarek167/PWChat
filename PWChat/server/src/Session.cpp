#include "server/Session.h"
#include "common/Packet.h"
#include <iostream>

using asio::ip::tcp;

Session::Session(tcp::socket socket)
    : m_socket(std::move(socket)){};

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
            Packet receivedPacket = Packet::unpack(rawData);
            std::cout << receivedPacket.header().signature << std::endl;
            std::cout << (int)receivedPacket.header().type << std::endl;
            std::cout << (int)receivedPacket.header().bodySize << std::endl;
            std::string message{receivedPacket.body().begin(), receivedPacket.body().end()};
            std::cout << message << std::endl;
            wait_for_request();
        } else {
            std::cout << "error: " << ec << std::endl;
        }
    });
}
