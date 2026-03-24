#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

class Server {
public:
    Server(boost::asio::io_context& io_context, short port)
        : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }
};
