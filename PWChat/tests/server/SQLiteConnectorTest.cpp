#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "server/include/server/SQLiteConnector.h"

class SQLiteConnectorTest : public ::testing::Test {
protected:
    SQLiteConnector db;
    std::string testDbPath = ":memory:";

    void SetUp() override {
        ASSERT_TRUE(db.connect(testDbPath));
        db.initializeSchema();
    }

    void TearDown() override {
        db.disconnect();
    }
};

TEST_F(SQLiteConnectorTest, Connect_MemoryDatabase_ReturnsTrue) {
    SQLiteConnector memoryDb;
    EXPECT_TRUE(memoryDb.connect(":memory:"));
    memoryDb.disconnect();
}

TEST_F(SQLiteConnectorTest, Connect_InvalidPath_ReturnsFalse) {
    SQLiteConnector invalidDb;
    EXPECT_FALSE(invalidDb.connect("/invalid_path/non_existent_dir/test.db"));
}

TEST_F(SQLiteConnectorTest, RegisterUser_ValidCredentials_ReturnsNewUserId) {
    uint32_t userId = db.registerUser("alice", "password123");
    EXPECT_GT(userId, 0u);
}

TEST_F(SQLiteConnectorTest, RegisterUser_DuplicateNickname_ReturnsZero) {
    uint32_t firstUserId = db.registerUser("bob", "pass1");
    uint32_t secondUserId = db.registerUser("bob", "pass2");

    EXPECT_GT(firstUserId, 0u);
    EXPECT_EQ(secondUserId, 0u);
}

TEST_F(SQLiteConnectorTest, LoginUser_ValidCredentials_ReturnsUserId) {
    uint32_t registeredId = db.registerUser("charlie", "secure_pass");
    uint32_t loggedInId = db.loginUser("charlie", "secure_pass");

    EXPECT_EQ(registeredId, loggedInId);
}

TEST_F(SQLiteConnectorTest, LoginUser_InvalidPassword_ReturnsZero) {
    db.registerUser("dan", "correct_pass");
    uint32_t loggedInId = db.loginUser("dan", "wrong_pass");

    EXPECT_EQ(loggedInId, 0u);
}

TEST_F(SQLiteConnectorTest, LoginUser_NonExistentUser_ReturnsZero) {
    uint32_t loggedInId = db.loginUser("nobody", "some_pass");
    EXPECT_EQ(loggedInId, 0u);
}

TEST_F(SQLiteConnectorTest, SaveRoom_ValidData_ReturnsNewRoomId) {
    uint32_t ownerId = db.registerUser("owner1", "pass");
    uint32_t roomId = db.saveRoom("General Chat", false, ownerId);

    EXPECT_GT(roomId, 0u);
}

TEST_F(SQLiteConnectorTest, SaveRoom_DuplicateRoomName_ReturnsZero) {
    uint32_t ownerId = db.registerUser("owner2", "pass");
    uint32_t firstRoomId = db.saveRoom("UniqueRoom", false, ownerId);
    uint32_t secondRoomId = db.saveRoom("UniqueRoom", true, ownerId);

    EXPECT_GT(firstRoomId, 0u);
    EXPECT_EQ(secondRoomId, 0u);
}

TEST_F(SQLiteConnectorTest, GetAllRooms_MultipleRoomsExist_ReturnsAllRooms) {
    uint32_t ownerId = db.registerUser("owner3", "pass");
    db.saveRoom("RoomA", false, ownerId);
    db.saveRoom("RoomB", true, ownerId);

    std::vector<RoomData> rooms = db.getAllRooms();
    ASSERT_EQ(rooms.size(), 2u);
    EXPECT_EQ(rooms[0].name, "RoomA");
    EXPECT_EQ(rooms[1].name, "RoomB");
}

TEST_F(SQLiteConnectorTest, SaveUserRoom_InsertValidRelation_ReturnsTrue) {
    uint32_t userId = db.registerUser("member1", "pass");
    uint32_t roomId = db.saveRoom("RoomC", false, userId);

    EXPECT_TRUE(db.saveUserRoom(userId, roomId, false));
}

TEST_F(SQLiteConnectorTest, GetUserRooms_UserBelongsToRooms_ReturnsAssociatedRooms) {
    uint32_t userId = db.registerUser("member2", "pass");
    uint32_t roomId1 = db.saveRoom("RoomD", false, userId);
    uint32_t roomId2 = db.saveRoom("RoomE", false, userId);

    db.saveUserRoom(userId, roomId1, false);
    db.saveUserRoom(userId, roomId2, true);

    std::vector<RoomData> rooms = db.getUserRooms(userId);
    ASSERT_EQ(rooms.size(), 2u);
}

TEST_F(SQLiteConnectorTest, DeleteUserRoom_ExistingRelation_ReturnsTrueAndRemovesRelation) {
    uint32_t userId = db.registerUser("member3", "pass");
    uint32_t roomId = db.saveRoom("RoomF", false, userId);
    db.saveUserRoom(userId, roomId, false);

    EXPECT_TRUE(db.deleteUserRoom(userId, roomId));
    std::vector<RoomData> rooms = db.getUserRooms(userId);
    EXPECT_TRUE(rooms.empty());
}

TEST_F(SQLiteConnectorTest, GetRoomUsers_FilterStandardAndAdmins_ReturnsCorrectSets) {
    uint32_t adminId = db.registerUser("admin_user", "pass");
    uint32_t standardId = db.registerUser("standard_user", "pass");
    uint32_t roomId = db.saveRoom("RoomG", false, adminId);

    db.saveUserRoom(adminId, roomId, true);
    db.saveUserRoom(standardId, roomId, false);

    std::vector<UserData> admins = db.getRoomUsers(roomId, true);
    std::vector<UserData> standardUsers = db.getRoomUsers(roomId, false);

    ASSERT_EQ(admins.size(), 1u);
    EXPECT_EQ(admins[0].nickname, "admin_user");

    ASSERT_EQ(standardUsers.size(), 1u);
    EXPECT_EQ(standardUsers[0].nickname, "standard_user");
}

TEST_F(SQLiteConnectorTest, AddAdmin_PromoteExistingUser_ReturnsTrueAndUpdatesStatus) {
    uint32_t userId = db.registerUser("candidate", "pass");
    uint32_t roomId = db.saveRoom("RoomH", false, userId);
    db.saveUserRoom(userId, roomId, false);

    EXPECT_TRUE(db.addAdmin(roomId, userId));
    std::vector<UserData> admins = db.getRoomUsers(roomId, true);
    ASSERT_EQ(admins.size(), 1u);
    EXPECT_EQ(admins[0].id, userId);
}

TEST_F(SQLiteConnectorTest, AddAdmin_NonExistentUserInRoom_ReturnsFalse) {
    uint32_t userId = db.registerUser("outsider", "pass");
    uint32_t roomId = db.saveRoom("RoomI", false, userId);

    EXPECT_FALSE(db.addAdmin(roomId, 99999u));
}

TEST_F(SQLiteConnectorTest, SaveMessage_RoomAndPrivateMessages_InsertsSuccessfully) {
    uint32_t user1 = db.registerUser("sender", "pass");
    uint32_t user2 = db.registerUser("receiver", "pass");
    uint32_t roomId = db.saveRoom("RoomJ", false, user1);

    EXPECT_TRUE(db.saveMessage(user1, roomId, "Hello room", MessageContentType::TEXT, true));
    EXPECT_TRUE(db.saveMessage(user1, user2, "Hello user", MessageContentType::TEXT, false));
}

TEST_F(SQLiteConnectorTest, GetMessages_RoomHistory_ReturnsOrderedMessages) {
    uint32_t senderId = db.registerUser("userA", "pass");
    uint32_t roomId = db.saveRoom("RoomK", false, senderId);

    db.saveMessage(senderId, roomId, "First", MessageContentType::TEXT, true);
    db.saveMessage(senderId, roomId, "Second", MessageContentType::TEXT, true);

    std::vector<MessageData> messages = db.getMessages(roomId, senderId, true, 10, 0);
    ASSERT_EQ(messages.size(), 2u);
    EXPECT_EQ(messages[1].message, "First");
    EXPECT_EQ(messages[0].message, "Second");
}

TEST_F(SQLiteConnectorTest, GetMessages_PrivateHistory_ReturnsBiDirectionalConversation) {
    uint32_t aliceId = db.registerUser("alice_chat", "pass");
    uint32_t bobId = db.registerUser("bob_chat", "pass");

    db.saveMessage(aliceId, bobId, "Hi Bob", MessageContentType::TEXT, false);
    db.saveMessage(bobId, aliceId, "Hi Alice", MessageContentType::TEXT, false);

    std::vector<MessageData> historyForAlice = db.getMessages(bobId, aliceId, false, 10, 0);
    std::vector<MessageData> historyForBob = db.getMessages(aliceId, bobId, false, 10, 0);

    ASSERT_EQ(historyForAlice.size(), 2u);
    EXPECT_EQ(historyForAlice[1].message, "Hi Bob");
    EXPECT_EQ(historyForAlice[0].message, "Hi Alice");
    EXPECT_EQ(historyForAlice.size(), historyForBob.size());
}

TEST_F(SQLiteConnectorTest, SaveAndGetRoomCode_ValidCode_StoredAndRetrievedBeforeExpiration) {
    uint32_t ownerId = db.registerUser("coder", "pass");
    uint32_t roomId = db.saveRoom("PrivateRoom", true, ownerId);

    EXPECT_TRUE(db.saveRoomCode(roomId, 123456u));
    EXPECT_EQ(db.getRoomCode(roomId), 123456u);
}

TEST_F(SQLiteConnectorTest, GetRoomCode_NonExistentRoom_ReturnsZero) {
    EXPECT_EQ(db.getRoomCode(99999u), 0u);
}

TEST_F(SQLiteConnectorTest, GetUsername_ExistingAndNonExistentId_ReturnsCorrectOutputs) {
    uint32_t userId = db.registerUser("test_subject", "pass");

    EXPECT_EQ(db.getUsername(userId), "test_subject");
    EXPECT_EQ(db.getUsername(99999u), "");
}

TEST_F(SQLiteConnectorTest, GetLastUserPrivChats_ActiveConversations_ReturnsDistinctPartnersInCorrectOrder) {
    uint32_t userA = db.registerUser("user_a", "pass");
    uint32_t userB = db.registerUser("user_b", "pass");
    uint32_t userC = db.registerUser("user_c", "pass");

    db.saveMessage(userA, userB, "Message 1", MessageContentType::TEXT, false);
    db.saveMessage(userA, userC, "Message 2", MessageContentType::TEXT, false);

    std::vector<UserData> chats = db.getLastUserPrivChats(userA);
    ASSERT_EQ(chats.size(), 2u);
    EXPECT_EQ(chats[1].id, userC);
    EXPECT_EQ(chats[0].id, userB);
}

TEST_F(SQLiteConnectorTest, FindUserByNick_ExistingAndMissing_ReturnsExpectedIds) {
    uint32_t registeredId = db.registerUser("search_me", "pass");

    EXPECT_EQ(db.findUserByNick("search_me"), registeredId);
    EXPECT_EQ(db.findUserByNick("missing_user"), 0u);
}
