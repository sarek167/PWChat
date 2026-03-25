#include "server/Session.h"
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
            std::string data{
                std::istreambuf_iterator<char>(&m_buffer),
                    std::istreambuf_iterator<char>()
            };
            std::cout << data << std::endl;
            wait_for_request();
        } else {
            std::cout << "error: " << ec << std::endl;
        }
    });
}
