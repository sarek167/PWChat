#ifndef SESSION_H
#define SESSION_H

#include <asio.hpp>
#include "common/User.h"
#include "common/Packet.h"

using asio::ip::tcp;

class Server;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, Server& server);

    uint32_t userId();
    std::shared_ptr<User> user();
    void setUser(uint32_t id, std::string nickname);
    void doRead();
    void deliver(const Packet& p);

private:
    tcp::socket m_socket;
    asio::streambuf m_buffer;
    Server& m_server;
    std::shared_ptr<User> m_user;
    void waitForRequest();
    void readBody(PacketHeader header);
};

#endif // SESSION_H
