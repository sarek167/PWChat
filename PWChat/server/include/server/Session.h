#ifndef SESSION_H
#define SESSION_H

#include <asio.hpp>
#include <memory>
#include <stdint.h>
#include <string>
#include "common/User.h"
#include "common/Packet.h"

using asio::ip::tcp;

class Server;

/**
 * @class Session
 * @brief Represents a single active client connection on the server.
 * * This class inherits from std::enable_shared_from_this so we can safely
 * pass shared pointers of this session to rooms or async Asio handlers.
 * It reads data from the socket, authenticates the user, and sends packets back.
 */
class Session : public std::enable_shared_from_this<Session> {
public:
    /**
     * @brief Constructor for Session class.
     * @param socket The TCP socket connected to the client.
     * @param server Reference to the main Server instance to access managers or database.
     */
    Session(tcp::socket socket, Server& server);

    /**
     * @brief Destructor for Session class.
     * Closes the connection and cleans up resources.
     */
    ~Session() = default;

    /**
     * @brief Helper getter to directly grab the connected user's ID.
     * @return Unique ID of the logged-in user, or 0 if not logged in.
     */
    uint32_t userId();

    /**
     * @brief Getter for the User object linked to this session.
     * @return Shared pointer to the User instance.
     */
    std::shared_ptr<User> user();

    /**
     * @brief Populates the User object data once the client logs in or registers successfully.
     * @param id The user ID received from the database.
     * @param nickname The username of the client.
     */
    void setUser(uint32_t id, std::string nickname);

    /**
     * @brief Starts the asynchronous reading loop to listen for data from the client.
     */
    void doRead();

    /**
     * @brief Sends a network packet directly to this specific client.
     * @param p The Packet object to serialize and transmit.
     */
    void deliver(const Packet& p);

    /**
     * @brief Checks if the client has successfully logged in.
     * @return true if authenticated, false otherwise.
     */
    bool isAuthenticated() const;

    /**
     * @brief Logs out the user by resetting the authentication flag and clearing user data.
     */
    void logout();

private:
    /**
     * @brief Asynchronously waits for a packet header (fixed size) to arrive from the network.
     */
    void waitForRequest();

    /**
     * @brief Asynchronously reads the packet body from the socket based on the size defined in the header.
     * @param header The PacketHeader containing metadata about the incoming payload size and type.
     */
    void readBody(PacketHeader header);

    tcp::socket m_socket;               /**< The network socket used for communication with this client. */
    asio::streambuf m_buffer;           /**< Internal buffer where incoming raw bytes are stored temporarily. */
    Server& m_server;                   /**< Reference to the main server instance for routing messages. */
    std::shared_ptr<User> m_user;       /**< Pointer to the User object containing details like ID and nickname. */
    bool m_isAuthenticated = false;     /**< Security flag tracking whether this session is successfully logged in. */
};

#endif // SESSION_H
