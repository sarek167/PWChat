#ifndef SERVER_H
#define SERVER_H

#include <asio.hpp>

using asio::ip::tcp;

class Server {
public:
    Server(asio::io_context& io_context, short port);
private:
    void do_accept();
    tcp::acceptor m_acceptor;
};

#endif // SERVER_H
