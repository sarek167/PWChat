#include "server/Server.h"
#include "server/Session.h"
#include <iostream>

using asio::ip::tcp;

Server::Server(asio::io_context& io_context, short port)
        : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

void Server::do_accept() {
    m_acceptor.async_accept([this](asio::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::cout << "creatin session on: "
                      << socket.remote_endpoint().address().to_string()
                      << ":" << socket.remote_endpoint().port() << "\n";
            std::make_shared<Session>(std::move(socket)) -> do_read();
        } else {
            std::cout << "error: " << ec.message() << std::endl;
        }

        do_accept();
    });
}

