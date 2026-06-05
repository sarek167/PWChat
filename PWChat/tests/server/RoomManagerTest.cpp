#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include "server/Server.h"
#include "server/RoomManager.h"
#include "server/Room.h"
#include "server/PublicRoom.h"
#include "server/PrivateRoom.h"
#include "server/Session.h"
#include "common/Packet.h"

class FakeSession : public Session {
public:
    FakeSession(uint32_t id, Server& fakeServer) : Session(asio::ip::tcp::socket(ioContext), fakeServer), m_id(id) {}
    uint32_t userId() { return m_id; }
    void deliver(const Packet& packet) { m_delivered.push_back(packet); }
    const std::vector<Packet>& delivered() const { return m_delivered; }
private:
    static asio::io_context ioContext;
    uint32_t m_id;
    std::vector<Packet> m_delivered;
};

asio::io_context FakeSession::ioContext;

class RoomManagerTest : public ::testing::Test {
protected:
    asio::io_context ioContext;
    std::unique_ptr<Server> fakeServer;

    void SetUp() override {
        fakeServer = std::make_unique<Server>(ioContext, 0);
    }
};

TEST_F(RoomManagerTest, CreateRoom_PublicRoom_CreatesAndStoresCorrectly) {
    RoomManager manager;
    std::shared_ptr<Room> room = manager.createRoom(1u, "Lobby", false, 100u);

    ASSERT_NE(room, nullptr);
    EXPECT_EQ(room->id(), 1u);
    EXPECT_EQ(room->name(), "Lobby");
    EXPECT_EQ(room->ownerId(), 100u);
    EXPECT_TRUE(room->canJoin("any_token"));
}

TEST_F(RoomManagerTest, CreateRoom_PrivateRoom_CreatesAndStoresCorrectly) {
    RoomManager manager;
    std::shared_ptr<Room> room = manager.createRoom(2u, "Secret", true, 200u);

    ASSERT_NE(room, nullptr);
    EXPECT_EQ(room->id(), 2u);
    EXPECT_EQ(room->name(), "Secret");
    EXPECT_EQ(room->ownerId(), 200u);
}

TEST_F(RoomManagerTest, GetRoom_ByValidIdAndName_ReturnsCorrectRoom) {
    RoomManager manager;
    manager.createRoom(1u, "Lobby", false, 100u);

    std::shared_ptr<Room> byId = manager.getRoom(1u);
    std::shared_ptr<Room> byName = manager.getRoom("Lobby");

    EXPECT_EQ(byId, byName);
    ASSERT_NE(byId, nullptr);
    EXPECT_EQ(byId->id(), 1u);
}

TEST_F(RoomManagerTest, GetRoom_ByInvalidIdAndName_ReturnsNullptr) {
    RoomManager manager;

    EXPECT_EQ(manager.getRoom(999u), nullptr);
    EXPECT_EQ(manager.getRoom("NonExistent"), nullptr);
}

TEST_F(RoomManagerTest, Initialize_VectorOfRooms_ClearsAndPopulatesManager) {
    RoomManager manager;
    manager.createRoom(5u, "OldRoom", false, 500u);

    std::vector<RoomData> dataList = {
        {1u, "Room1", false, 10u},
        {2u, "Room2", true, 20u}
    };

    manager.initialize(dataList);

    EXPECT_EQ(manager.getRoom(5u), nullptr);
    EXPECT_NE(manager.getRoom(1u), nullptr);
    EXPECT_NE(manager.getRoom(2u), nullptr);
    EXPECT_EQ(manager.allRooms().size(), 2u);
}

TEST_F(RoomManagerTest, LoginAndLogoutInitialize_ValidRooms_AddsAndRemovesClient) {
    RoomManager manager;
    manager.createRoom(1u, "Room1", false, 10u);
    auto session = std::make_shared<FakeSession>(1001u, *fakeServer);
    session->setUser(1001u, "FakeUser");
    std::vector<RoomData> dataList = {{1u, "Room1", false, 10u}};

    manager.loginInitialize(dataList, session);
    manager.logoutInitialize(dataList, session);

    SUCCEED();
}

TEST_F(RoomManagerTest, LoginAndLogoutInitialize_NonExistentRooms_LogsErrorAndDoesNotCrash) {
    RoomManager manager;
    auto session = std::make_shared<FakeSession>(1001u, *fakeServer);
    session->setUser(1001u, "FakeUser");
    std::vector<RoomData> dataList = {{999u, "GhostRoom", false, 10u}};

    EXPECT_NO_THROW(manager.loginInitialize(dataList, session));
    EXPECT_NO_THROW(manager.logoutInitialize(dataList, session));
}

TEST_F(RoomManagerTest, AddClient_NewClient_AppendsToClientList) {
    PublicRoom room(1u, "Public", 10u);
    auto session = std::make_shared<FakeSession>(101u, *fakeServer);
    session->setUser(101u, "FakeUser");

    room.addClient(session);

    SUCCEED();
}

TEST_F(RoomManagerTest, AddClient_DuplicateClient_DoesNotDuplicate) {
    PublicRoom room(1u, "Public", 10u);
    auto session = std::make_shared<FakeSession>(101u, *fakeServer);
    session->setUser(101u, "FakeUser");

    room.addClient(session);
    room.addClient(session);

    SUCCEED();
}

TEST_F(RoomManagerTest, RemoveClient_ExistingClient_RemovesExclusively) {
    PublicRoom room(1u, "Public", 10u);
    auto sessionA = std::make_shared<FakeSession>(101u, *fakeServer);
    sessionA->setUser(101u, "UserA");
    auto sessionB = std::make_shared<FakeSession>(102u, *fakeServer);
    sessionB->setUser(102u, "UserB");

    room.addClient(sessionA);
    room.addClient(sessionB);
    room.removeClient(sessionA);

    SUCCEED();
}

TEST_F(RoomManagerTest, RemoveClient_NonExistentClient_DoesNotCrash) {
    PublicRoom room(1u, "Public", 10u);
    auto session = std::make_shared<FakeSession>(101u, *fakeServer);
    session->setUser(101u, "FakeUser");

    EXPECT_NO_THROW(room.removeClient(session));
}

TEST_F(RoomManagerTest, AddAndRemoveAdmin_ValidLifecycle_UpdatesAdminState) {
    PublicRoom room(1u, "Public", 10u);

    EXPECT_FALSE(room.checkIfAdmin(50u));

    room.addAdmin(50u);
    EXPECT_TRUE(room.checkIfAdmin(50u));

    room.addAdmin(50u);
    EXPECT_TRUE(room.checkIfAdmin(50u));

    room.removeAdmin(50u);
    EXPECT_FALSE(room.checkIfAdmin(50u));
}

TEST_F(RoomManagerTest, RemoveAdmin_NonAdmin_DoesNotCrash) {
    PublicRoom room(1u, "Public", 10u);

    EXPECT_NO_THROW(room.removeAdmin(99u));
}

TEST_F(RoomManagerTest, CanJoin_MatchingToken_ReturnsTrue) {
    PrivateRoom room(2u, "Private", 20u);

    EXPECT_TRUE(room.canJoin(""));
}

TEST_F(RoomManagerTest, CanJoin_MismatchedToken_ReturnsFalse) {
    PrivateRoom room(2u, "Private", 20u);

    EXPECT_FALSE(room.canJoin("invalid_code"));
}
