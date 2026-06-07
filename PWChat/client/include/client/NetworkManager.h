#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include <asio.hpp>
#include <vector>
#include <QObject>
#include <QString>
#include "common/Packet.h"
#include "common/User.h"
#include "common/RoomData.h"
#include "common/RegisterRequest.h"
#include "common/RoomUserData.h"
#include "common/MessageData.h"
#include "common/JoinRoomRequest.h"
#include "common/AuthResponse.h"

using asio::ip::tcp;

/**
 * @class NetworkManager
 * @brief Class responsible for asynchronous network communication with server.
 * * Manages TCP connections, packet reading loop and packaging and sending
 * data structures. Informs the rest of application about received packets via Qt signals.
 */
class NetworkManager : public QObject {
    Q_OBJECT
    friend class NetworkManagerTest;
public:
    /**
     * @brief Constructor of NetworkManager class. It assigns m_socket and m_resolver.
     * @param parent Pointer to the parent QObject (defaults to nullptr).
     */
    explicit NetworkManager(QObject *parent = nullptr)
        : QObject(parent), m_socket(m_io_context), m_resolver(m_io_context) {}

    /**
     * @brief Destructor of NetworkManager class.
     * Closes the active socket and stops the internal io_context.
     */
    ~NetworkManager();

    /**
     * @brief Connects client to the server based on given host and port.
     * @param host The server hostname or IP address (e.g., "127.0.0.1").
     * @param port The server port number as a string (e.g., "12345").
     */
    void connect(const std::string& host, const std::string& port);

    /**
     * @brief Sends asynchronously packet p to the server, where it is routed properly.
     * @param p The network packet structure to be serialized and sent.
     */
    void send(const Packet& p);

    /**
     * @brief Method which starts process of listening for received packets.
     */
    void doRead();

    /**
     * @brief Getter for user related to the current session.
     * @return std::shared_ptr<User> Shared pointer to the current User object.
     */
    std::shared_ptr<User> user();

    /**
     * @brief Setter for user related to the current session.
     * @param userPtr Shared pointer to the new User object to set.
     */
    void setUser(std::shared_ptr<User> userPtr);

private:
    /**
     * @brief Asynchronously waits for receiving packet from the server.
     * When received it consumes bytes of header size from buffer and calls method readBody.
     */
    void waitForRequest();

    /**
     * @brief Receives unpacked packet and performs proper action, depending on the packet message type in header.
     * @param packet The complete unpacked Packet containing type, header, and raw data payload.
     */
    void dispatchPacket(const Packet& packet);

    /**
     * @brief Asynchronously reads bytes of packet size (known from passed header) from buffer.
     * Creates packet and calls method dispatchPacket. Finally, calls waitForRequest to start the process again.
     * @param header The PacketHeader object containing metadata about incoming payload size and type.
     */
    void readBody(PacketHeader header);

    std::shared_ptr<User> m_user;  /**< Pointer to the user related with current session */
    asio::io_context m_io_context; /**< Asio context of client */
    tcp::socket m_socket;          /**< Opened socket for the current session */
    tcp::resolver m_resolver;      /**< Resolver used to resolve server connection request. */
    asio::streambuf m_buffer;      /**< Asio buffer for transferring bytes between client and server. */

signals:
    /**
     * @brief Emitted when an authentication response is received from the server.
     * @param res The authentication response object containing user details and rooms.
     */
    void AuthResultReceived(const AuthResponse& res);

    /**
     * @brief Emitted when a text or system message is received from a chat or a room.
     * @param senderId Unique identifier of the message sender.
     * @param senderName Nickname of the message sender.
     * @param targetId ID of the target user or room receiving the message.
     * @param msgType The content type of the message (e.g., text, image).
     * @param message The text content of the received message.
     * @param toRoom Flag specifying if the message was sent to a room (true) or private chat (false).
     */
    void MessageReceived(const uint32_t senderId, const QString& senderName, const uint32_t targetId, const MessageContentType& msgType, const QString& message, bool toRoom);

    /**
     * @brief Emitted when a voice or audio packet is received from another user.
     * @param senderId String representation of the sender's unique identifier.
     * @param audioMessage Vector containing raw compressed Opus audio bytes.
     */
    void AudioMessageReceived(const QString& senderId, const std::vector<char>& audioMessage);

    /**
     * @brief Emitted when a room creation or room join request is confirmed by the server.
     * @param room The structural data of the validated room.
     */
    void RoomRequestConfirmation(const RoomData& room);

    /**
     * @brief Emitted when a registration result packet is received from the server.
     * @param req The register request object reflecting the outcome of the action.
     */
    void RegisterResultReceived(const RegisterRequest& req);

    /**
     * @brief Emitted when the user is successfully logged out of the application server.
     */
    void LogoutResultReceived();

    /**
     * @brief Emitted when specific room information and member list are received.
     * @param roomUserData Detailed room object containing lists of users and configurations.
     */
    void RoomInfoReceived(const RoomUserData& roomUserData);

    /**
     * @brief Emitted when confirmation of a user leaving a room is received.
     * @param roomId Unique identifier of the room left.
     * @param userId Unique identifier of the user who left the room.
     */
    void LeaveResultReceived(const uint32_t roomId, const uint32_t userId);

    /**
     * @brief Emitted when a requested history batch of older messages is received.
     * @param messages Vector containing archived data for chat message logs.
     */
    void MessagesReceived(const std::vector<MessageData>& messages);

    /**
     * @brief Emitted when a newly generated invite or access code for a private room is received.
     * @param code The numeric password or access code.
     */
    void AccessCodeReceived(const uint32_t& code);

    /**
     * @brief Emitted when attempting to join a private room that requires a passcode verification.
     * @param req The join room request context triggering the pin challenge.
     */
    void AccessCodeRequired(JoinRoomRequest req);

    /**
     * @brief Emitted when the result of a global database user search is received.
     * @param foundUser Structure containing data profile of the discovered contact.
     */
    void UserFoundResult(const UserData& foundUser);
};

#endif // NETWORKMANAGER_H
