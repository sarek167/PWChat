#ifndef SESSION_H
#define SESSION_H

#include <asio.hpp>

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket);
    void do_read();

private:
    tcp::socket m_socket;
    asio::streambuf m_buffer;
    // TO DO: add shared_ptr<User>
    void wait_for_request();
};

#endif // SESSION_H
