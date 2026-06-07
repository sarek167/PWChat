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
 * @brief Class responsible for handling network communication with the server.
 * * It manages TCP connections, runs the packet reading loop, and sends data.
 * It uses Qt signals to notify the rest of the application when new packets arrive.
 */
class NetworkManager : public QObject {
    Q_OBJECT
    friend class NetworkManagerTest;
public:
    /**
     * @brief Constructor for NetworkManager class. Initializes the socket and resolver.
     * @param parent Pointer to the parent QObject (defaults to nullptr).
     */
    explicit NetworkManager(QObject *parent = nullptr)
        : QObject(parent), m_socket(m_io_context), m_resolver(m_io_context) {}

    /**
     * @brief Destructor for NetworkManager class.
     * Closes the open socket and stops the internal io_context loop.
     */
    ~NetworkManager();

    /**
     * @brief Connects the client to the server using the given host and port.
     * @param host The server address or hostname (e.g., "127.0.0.1").
     * @param port The server port number as a string (e.g., "12345").
     */
    void connect(const std::string& host, const std::string& port);

    /**
     * @brief Sends a packet to the server.
     * @param p The network packet object to be sent.
     */
    void send(const Packet& p);

    /**
     * @brief Starts the background process of listening for incoming packets.
     */
    void doRead();

    /**
     * @brief Getter for the user linked to the current session.
     * @return std::shared_ptr<User> Shared pointer to the current User.
     */
    std::shared_ptr<User> user();

    /**
     * @brief Setter to update the user linked to the current session.
     * @param userPtr Shared pointer to the new User object.
     */
    void setUser(std::shared_ptr<User> userPtr);

private:
    /**
     * @brief Waits for a new packet header from the server.
     * Once the header bytes are read, it calls readBody to handle the rest of the packet.
     */
    void waitForRequest();

    /**
     * @brief Takes an unpacked packet and triggers the right action based on its message type.
     * @param packet The full Packet object containing the header and body data.
     */
    void dispatchPacket(const Packet& packet);

    /**
     * @brief Reads the remaining bytes of the packet body based on the size found in the header.
     * Reconstructs the packet, calls dispatchPacket, and goes back to waiting for the next header.
     * @param header The PacketHeader containing details about the incoming data size and type.
     */
    void readBody(PacketHeader header);

    std::shared_ptr<User> m_user;  /**< Pointer to the current user of this app session. */
    asio::io_context m_io_context; /**< Asio context loop for the client. */
    tcp::socket m_socket;          /**< The open network socket for this session. */
    tcp::resolver m_resolver;      /**< Resolver used to look up the server address. */
    asio::streambuf m_buffer;      /**< Internal buffer used to store bytes received from the server. */

signals:
    /**
     * @brief Emitted when the server responds to a login or registration attempt.
     * @param res Object containing the user's details, chat lists, and rooms.
     */
    void AuthResultReceived(const AuthResponse& res);

    /**
     * @brief Emitted when a new chat message arrives.
     * @param senderId ID of the user who sent the message.
     * @param senderName Nickname of the sender.
     * @param targetId ID of the receiving room or user.
     * @param msgType The type of message (text or audio).
     * @param message The actual message text or filename.
     * @param toRoom True if the message belongs to a room, false if it's a private chat.
     */
    void MessageReceived(const uint32_t senderId, const QString& senderName, const uint32_t targetId, const MessageContentType& msgType, const QString& message, bool toRoom);

    /**
     * @brief Emitted when raw audio data (voice message) is received.
     * @param senderId ID of the user who sent the audio.
     * @param audioMessage Vector containing the raw audio bytes (e.g., Opus data).
     */
    void AudioMessageReceived(const QString& senderId, const std::vector<char>& audioMessage);

    /**
     * @brief Emitted when the server confirms that a room was successfully created or joined.
     * @param room Object containing the basic details of the room.
     */
    void RoomRequestConfirmation(const RoomData& room);

    /**
     * @brief Emitted when the registration result arrives from the server.
     * @param req The original registration request info, reflecting the outcome.
     */
    void RegisterResultReceived(const RegisterRequest& req);

    /**
     * @brief Emitted when the user is successfully logged out.
     */
    void LogoutResultReceived();

    /**
     * @brief Emitted when full details about a room (like its member list) are received.
     * @param roomUserData Object holding lists of users, admins, and room settings.
     */
    void RoomInfoReceived(const RoomUserData& roomUserData);

    /**
     * @brief Emitted when the server confirms that a user left a room.
     * @param roomId ID of the room that was left.
     * @param userId ID of the user who left.
     */
    void LeaveResultReceived(const uint32_t roomId, const uint32_t userId);

    /**
     * @brief Emitted when a requested batch of archived chat history arrives.
     * @param messages Vector containing the list of past messages.
     */
    void MessagesReceived(const std::vector<MessageData>& messages);

    /**
     * @brief Emitted when a new password/pin code for a private room is received.
     * @param code The numeric access code.
     */
    void AccessCodeReceived(const uint32_t& code);

    /**
     * @brief Emitted when trying to enter a private room that requires typing a password/pin.
     * @param req The join room request that needs verification.
     */
    void AccessCodeRequired(JoinRoomRequest req);

    /**
     * @brief Emitted when the server returns the result of searching for a user.
     * @param foundUser Structure containing the basic profile of the found user.
     */
    void UserFoundResult(const UserData& foundUser);
};

#endif // NETWORKMANAGER_H
