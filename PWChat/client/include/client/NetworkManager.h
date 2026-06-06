#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include <asio.hpp>
#include "common/Packet.h"
#include <QObject>
#include "common/User.h"
#include "common/RoomData.h"
#include "common/RegisterRequest.h"
#include "common/RoomUserData.h"
#include "common/MessageData.h"
#include "common/JoinRoomRequest.h"
#include "common/AuthResponse.h"

using asio::ip::tcp;

class NetworkManager : public QObject {
    Q_OBJECT
    friend class NetworkManagerTest;
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
    void dispatchPacket(const Packet& packet);
    void readBody(PacketHeader header);
    std::shared_ptr<User> m_user;
    asio::io_context m_io_context;
    tcp::socket m_socket;
    tcp::resolver m_resolver;
    asio::streambuf m_buffer;

signals:
    void AuthResultReceived(const AuthResponse& res);
    void MessageReceived(const uint32_t senderId, const QString& senderName, const uint32_t targetId, const MessageContentType& msgType, const QString& message, bool toRoom);
    void AudioMessageReceived(const QString& senderId, const std::vector<char>& audioMessage);
    void RoomRequestConfirmation(const RoomData& room);
    void RegisterResultReceived(const RegisterRequest& req);
    void LogoutResultReceived();
    void RoomInfoReceived(const RoomUserData& roomUserData);
    void LeaveResultReceived(const uint32_t roomId, const uint32_t userId);
    void MessagesReceived(const std::vector<MessageData>& messages);
    void AccessCodeReceived(const uint32_t& code);
    void AccessCodeRequired(JoinRoomRequest req);
    void UserFoundResult(const UserData& foundUser);
};

#endif // NETWORKMANAGER_H
