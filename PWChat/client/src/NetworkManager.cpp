#include "client/NetworkManager.h"
#include <iostream>

NetworkManager::~NetworkManager() {
    if (m_socket.is_open()) {
        asio::error_code ec;
        m_socket.close(ec);
    }
    m_io_context.stop();
}

void NetworkManager::connect(const std::string& host, const std::string& port) {
    asio::connect(m_socket, m_resolver.resolve(host, port));
    std::string data{"some client data...\0"};
    auto result = asio::write(m_socket, asio::buffer(data));

    std::cout << "data sent:" << data.length() << '/' << result << std::endl;

    std::error_code ec;
    m_socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
}
