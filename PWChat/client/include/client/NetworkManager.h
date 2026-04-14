#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include <asio.hpp>
#include "common/Packet.h"
#include <QObject>
#include "common/User.h"

using asio::ip::tcp;

class NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr)
        : QObject(parent), m_socket(m_io_context), m_resolver(m_io_context) {}
    ~NetworkManager();
    void connect(const std::string& host, const std::string& port);
    void send(const Packet& p);
    void doRead();
    std::shared_ptr<User> user();
    void setUser(std::shared_ptr<User> userPtr);

private:
    void waitForRequest();
    void readBody(PacketHeader header);
    std::shared_ptr<User> m_user;
    asio::io_context m_io_context;
    tcp::socket m_socket;
    tcp::resolver m_resolver;
    asio::streambuf m_buffer;

signals:
    void AuthResultReceived(std::string status);
    void MessageReceived(const QString& senderId, const QString& message);
};

#endif // NETWORKMANAGER_H
