#ifndef SERVER_H
#define SERVER_H

#include <asio.hpp>
#include <map>
#include <memory>
#include <mutex>
#include <stdint.h>
#include "Session.h"
#include "common/Packet.h"
#include "server/RoomManager.h"
#include "server/Commands/Command.h"
#include "server/DBConnector.h"

using asio::ip::tcp;

/**
 * @class Server
 * @brief The main class of the server core application.
 * * It is responsible for accepting new client connections, managing the list
 * of active users, loading initial database records, and routing incoming network packets.
 */
class Server {
public:
    /**
     * @brief Constructor for Server class.
     * Sets up the TCP acceptor on the specified port.
     * @param io_context Asio context loop used for handling network events.
     * @param port The port number on which the server will listen for connections.
     */
    Server(asio::io_context& io_context, short port);

    /**
     * @brief Destructor for Server class.
     */
    ~Server() = default;

    /**
     * @brief Triggered whenever a complete packet is received from a client session.
     * Passes the packet to the appropriate Command handler.
     * @param session Shared pointer to the client session that sent the packet.
     * @param p The complete incoming Packet object.
     */
    void onPacketReceived(std::shared_ptr<Session> session, const Packet& p);

    /**
     * @brief Routes a packet directly to its target user if they are online.
     * Used mainly for direct messages between two users.
     * @param p The packet to be routed.
     */
    void routePacket(const Packet& p);

    /**
     * @brief Adds a newly authenticated client session to the active clients list.
     * @param session Shared pointer to the client session.
     */
    void insertClient(std::shared_ptr<Session> session);

    /**
     * @brief Removes a client session from the active list (e.g., when they disconnect).
     * @param session Shared pointer to the client session to remove.
     */
    void removeClient(std::shared_ptr<Session> session);

    /**
     * @brief Loads baseline configuration data (like rooms) from the database at startup.
     */
    void loadDataFromDB();

    /**
     * @brief Finds an active client session using their unique user ID.
     * @param clientId The ID of the user to look for.
     * @return Shared pointer to the user's Session, or nullptr if they are offline.
     */
    const std::shared_ptr<Session> client(uint32_t clientId);

    /**
     * @brief Getter for the room manager subsystem reference.
     * @return Reference to the internal RoomManager instance.
     */
    RoomManager& roomManager();

    /**
     * @brief Getter for the database connector subsystem reference.
     * @return Reference to the internal DBConnector instance.
     */
    DBConnector& db();

protected:
    RoomManager m_roomManager;         /**< Subsystem managing all active chat room channels. */
    std::unique_ptr<DBConnector> m_db; /**< Pointer to the interface executing database operations. */

private:
    /**
     * @brief Starts an asynchronous loop to wait for and accept incoming TCP client connections.
     */
    void do_accept();

    tcp::acceptor m_acceptor;                              /**< Asio TCP acceptor object used to listen for new connections. */
    std::map<uint32_t, std::shared_ptr<Session>> m_clients; /**< Map tracking currently logged-in users by their ID. */
    std::mutex m_clientsMutex;                             /**< Mutex protecting the clients map from race conditions in multi-threading. */
    std::map<MessageType, std::unique_ptr<Command>> m_commands; /**< Command pattern map routing message types to handler classes. */
};

#endif // SERVER_H
