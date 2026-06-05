#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include "server/Server.h"
#include "server/Session.h"
#include "common/Packet.h"

class MockDBConnector : public DBConnector {
public:
    bool connect(const std::string& dbPath) override { return true; }
    void disconnect() override {}
    void initializeSchema() override {}
    std::vector<RoomData> getAllRooms() override { return mockRooms; }
    int saveRoom(const std::string& name, const bool isPrivate, const uint32_t ownerId) override { return 0; }
    std::vector<RoomData> getUserRooms(const uint32_t userId) override { return {}; }
    bool saveUserRoom(const uint32_t userId, const uint32_t roomId, bool isAdmin) override { return true; }
    bool deleteUserRoom(const uint32_t userId, const uint32_t roomId) override { return true; }
    uint32_t registerUser(const std::string& nickname, const std::string& password) override { return 0; }
    uint32_t loginUser(const std::string& nickname, const std::string& password) override { return 0; }
    std::vector<UserData> getRoomUsers(const uint32_t roomId, bool getAdmins) override { return {}; }
    bool addAdmin(const uint32_t roomId, const uint32_t userId) override { return true; }
    bool saveMessage(const uint32_t senderId, const uint32_t targetId, const std::string& message, const MessageContentType& type, bool toRoom) override { return true; }
    std::vector<MessageData> getMessages(const uint32_t targetId, const uint32_t senderId, bool fromRoom, const int limit, const int offset) override { return {}; }
    bool saveRoomCode(const uint32_t roomId, const uint32_t code) override { return true; }
    uint32_t getRoomCode(const uint32_t roomId) override { return 0; }
    std::string getUsername(const uint32_t userId) override { return ""; }
    std::vector<UserData> getLastUserPrivChats(const uint32_t userId) override { return {}; }
    uint32_t findUserByNick(const std::string username) override { return 0; }

    std::vector<RoomData> mockRooms;
};

class SpySession : public Session {
public:
    SpySession(asio::ip::tcp::socket socket, Server& server)
        : Session(std::move(socket), server), mockAuthenticated(false) {}

    void deliver(const Packet& packet) {
        deliveredPackets.push_back(packet);
    }

    bool isAuthenticated() const {
        return mockAuthenticated;
    }

    bool mockAuthenticated;
    std::vector<Packet> deliveredPackets;
};

class ServerTest : public ::testing::Test {
protected:
    asio::io_context ioContext;
    std::unique_ptr<Server> server;

    void SetUp() override {
        server = std::make_unique<Server>(ioContext, 0);
    }
};

TEST_F(ServerTest, ServerInitialization_DefaultState_CreatesDatabaseAndPreparesCommands) {
    EXPECT_NE(&server->db(), nullptr);
    EXPECT_NO_THROW(server->roomManager());
}

TEST_F(ServerTest, Client_NonExistentId_ReturnsNullptr) {
    EXPECT_EQ(server->client(99999u), nullptr);
}

TEST_F(ServerTest, InsertClient_ValidSession_AddsClientToActiveMap) {
    asio::ip::tcp::socket socket(ioContext);
    auto session = std::make_shared<SpySession>(std::move(socket), *server);
    session->setUser(101u, "SpyUser");

    server->insertClient(session);

    std::shared_ptr<Session> retrieved = server->client(101u);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->userId(), 101u);

    server->removeClient(session);
}

TEST_F(ServerTest, RemoveClient_ExistingClient_RemovesClientFromActiveMap) {
    asio::ip::tcp::socket socket(ioContext);
    auto session = std::make_shared<SpySession>(std::move(socket), *server);
    session->setUser(202u, "SpyUser2");

    server->insertClient(session);
    ASSERT_NE(server->client(202u), nullptr);

    server->removeClient(session);

    EXPECT_EQ(server->client(202u), nullptr);
}

TEST_F(ServerTest, RemoveClient_NullptrPassed_DoesNotCrash) {
    EXPECT_NO_THROW(server->removeClient(nullptr));
}

TEST_F(ServerTest, OnPacketReceived_AuthenticatedSessionProtectedRequest_ExecutesCommand) {
    asio::ip::tcp::socket socket(ioContext);
    auto session = std::make_shared<SpySession>(std::move(socket), *server);
    session->mockAuthenticated = true;

    PacketHeader header;
    header.type = MessageType::MESS_TO_USER;
    Packet packet(header, {});

    EXPECT_NO_THROW(server->onPacketReceived(session, packet));
}

TEST_F(ServerTest, OnPacketReceived_UnknownMessageType_LogsAndDoesNotCrash) {
    asio::ip::tcp::socket socket(ioContext);
    auto session = std::make_shared<SpySession>(std::move(socket), *server);
    session->mockAuthenticated = true;

    PacketHeader header;
    header.type = static_cast<MessageType>(9999);
    Packet packet(header, {});

    EXPECT_NO_THROW(server->onPacketReceived(session, packet));
}

class SessionTest : public ::testing::Test {
protected:
    asio::io_context ioContext;
    std::unique_ptr<Server> server;
    std::shared_ptr<Session> session;

    void SetUp() override {
        server = std::make_unique<Server>(ioContext, 0);
        asio::ip::tcp::socket socket(ioContext);
        session = std::make_shared<Session>(std::move(socket), *server);
    }
};

TEST_F(SessionTest, Constructor_InitialState_NotAuthenticatedAndUserNull) {
    EXPECT_FALSE(session->isAuthenticated());
    EXPECT_EQ(session->user(), nullptr);
}

TEST_F(SessionTest, SetUser_ValidData_SetsUserAndAuthenticatesSession) {
    session->setUser(42u, "Alice");

    EXPECT_TRUE(session->isAuthenticated());
    ASSERT_NE(session->user(), nullptr);
    EXPECT_EQ(session->userId(), 42u);
}

TEST_F(SessionTest, Logout_AuthenticatedSession_ClearsUserAndAuthentication) {
    session->setUser(77u, "Bob");
    ASSERT_TRUE(session->isAuthenticated());

    session->logout();

    EXPECT_FALSE(session->isAuthenticated());
    EXPECT_EQ(session->user(), nullptr);
}
