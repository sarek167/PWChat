#ifndef SERVER_H
#define SERVER_H

#include <asio.hpp>
#include <map>
#include "Session.h"
#include "common/Packet.h"

using asio::ip::tcp;

class Server {
public:
    Server(asio::io_context& io_context, short port);
    void routePacket(const Packet& p);
    void insertClient(std::shared_ptr<Session> session);
    const std::shared_ptr<Session> client(uint32_t clientId);
private:
    void do_accept();
    tcp::acceptor m_acceptor;
    std::map<uint32_t, std::shared_ptr<Session>> m_clients;
    std::mutex m_clientsMutex;
};

#endif // SERVER_H
