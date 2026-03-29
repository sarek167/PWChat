#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include <asio.hpp>
#include "common/Packet.h"

using asio::ip::tcp;

class NetworkManager {
public:
    NetworkManager() : m_socket(m_io_context), m_resolver(m_io_context) {}
    ~NetworkManager();
    void connect(const std::string& host, const std::string& port);
    void send(const Packet& p);
    void doRead();

private:
    void waitForRequest();
    void readBody(PacketHeader header);
    asio::io_context m_io_context;
    tcp::socket m_socket;
    tcp::resolver m_resolver;
    asio::streambuf m_buffer;
};

#endif // NETWORKMANAGER_H
