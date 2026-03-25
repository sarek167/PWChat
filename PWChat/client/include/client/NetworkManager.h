#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include <asio.hpp>

using asio::ip::tcp;

class NetworkManager {
public:
    NetworkManager() : m_socket(m_io_context), m_resolver(m_io_context) {}
    ~NetworkManager();
    void connect(const std::string& host, const std::string& port);

private:
    asio::io_context m_io_context;
    tcp::socket m_socket;
    tcp::resolver m_resolver;
};

#endif // NETWORKMANAGER_H
