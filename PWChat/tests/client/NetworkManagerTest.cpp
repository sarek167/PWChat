#include <gtest/gtest.h>
#include <QApplication>
#include <QSignalSpy>
#include <memory>

#include "client/NetworkManager.h"
#include "common/Packet.h"
#include "common/PacketHeader.h"
#include "common/User.h"
#include "common/AuthResponse.h"
#include "common/RegisterRequest.h"
#include "common/MessageData.h"
#include "common/RoomData.h"
#include "common/RoomUserData.h"
#include "common/LeaveRoomRequest.h"
#include "common/JoinRoomRequest.h"
#include "common/UserData.h"

class NetworkManagerTest : public ::testing::Test {
protected:
    NetworkManager* nm = nullptr;

    void SetUp() override {
        nm = new NetworkManager();
    }

    void TearDown() override {
        delete nm;
    }

    void dispatchPacket(const Packet& packet) {
        nm->dispatchPacket(packet);
    }
};

TEST_F(NetworkManagerTest, User_InitiallyNull) {
    EXPECT_EQ(nm->user(), nullptr);
}

TEST_F(NetworkManagerTest, SetUser_StoresUser) {
    auto user = std::make_shared<User>(42, "alice");
    nm->setUser(user);
    EXPECT_EQ(nm->user(), user);
}

TEST_F(NetworkManagerTest, SetUser_UserIdAndNicknameAccessible) {
    auto user = std::make_shared<User>(7, "bob");
    nm->setUser(user);
    EXPECT_EQ(nm->user()->id(), 7u);
    EXPECT_EQ(nm->user()->nickname(), "bob");
}

TEST_F(NetworkManagerTest, SetUser_OverwritesPreviousUser) {
    nm->setUser(std::make_shared<User>(1, "first"));
    nm->setUser(std::make_shared<User>(2, "second"));
    EXPECT_EQ(nm->user()->id(), 2u);
}

TEST_F(NetworkManagerTest, SetUser_NullptrClearsUser) {
    nm->setUser(std::make_shared<User>(1, "alice"));
    nm->setUser(nullptr);
    EXPECT_EQ(nm->user(), nullptr);
}

TEST_F(NetworkManagerTest, DispatchPacket_LoginRequest_EmitsAuthResultReceived) {
    AuthResponse res;
    res.myId = 5;
    res.myNickname = "alice";
    Packet p(MessageType::LOGIN_REQUEST, 5, 0, res);

    QSignalSpy spy(nm, &NetworkManager::AuthResultReceived);
    dispatchPacket(p);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(NetworkManagerTest, DispatchPacket_LoginRequest_EmittedAuthResponseHasCorrectData) {
    AuthResponse res;
    res.myId = 99;
    res.myNickname = "carol";
    Packet p(MessageType::LOGIN_REQUEST, 99, 0, res);

    QSignalSpy spy(nm, &NetworkManager::AuthResultReceived);
    dispatchPacket(p);

    ASSERT_EQ(spy.count(), 1);
    AuthResponse emitted = qvariant_cast<AuthResponse>(spy.at(0).at(0));
    EXPECT_EQ(emitted.myId, 99u);
    EXPECT_EQ(emitted.myNickname, "carol");
}

TEST_F(NetworkManagerTest, DispatchPacket_RegisterRequest_EmitsRegisterResultReceived) {
    RegisterRequest req;
    req.id = 10;
    req.nickname = "newUser";
    req.password = "";
    Packet p(MessageType::REGISTER_REQUEST, 10, 0, req);

    QSignalSpy spy(nm, &NetworkManager::RegisterResultReceived);
    dispatchPacket(p);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(NetworkManagerTest, DispatchPacket_RegisterRequest_SetsUserOnManager) {
    RegisterRequest req;
    req.id = 55;
    req.nickname = "dave";
    req.password = "";
    Packet p(MessageType::REGISTER_REQUEST, 55, 0, req);

    dispatchPacket(p);

    ASSERT_NE(nm->user(), nullptr);
    EXPECT_EQ(nm->user()->id(), 55u);
    EXPECT_EQ(nm->user()->nickname(), "dave");
}

TEST_F(NetworkManagerTest, DispatchPacket_MessToUser_EmitsMessageReceived) {
    MessageData msg;
    msg.senderId = 1;
    msg.targetId = 2;
    msg.senderName = "alice";
    msg.message = "hello";
    msg.messageType = MessageContentType::TEXT;
    Packet p(MessageType::MESS_TO_USER, 1, 2, msg);

    QSignalSpy spy(nm, &NetworkManager::MessageReceived);
    dispatchPacket(p);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(NetworkManagerTest, DispatchPacket_MessToRoom_EmitsMessageReceivedWithToRoomTrue) {
    MessageData msg;
    msg.senderId = 3;
    msg.targetId = 10;
    msg.senderName = "bob";
    msg.message = "hi room";
    msg.messageType = MessageContentType::TEXT;
    Packet p(MessageType::MESS_TO_ROOM, 3, 10, msg);

    QSignalSpy spy(nm, &NetworkManager::MessageReceived);
    dispatchPacket(p);

    ASSERT_EQ(spy.count(), 1);
    bool toRoom = spy.at(0).at(5).toBool();
    EXPECT_TRUE(toRoom);
}

TEST_F(NetworkManagerTest, DispatchPacket_CreateRoomComm_EmitsRoomRequestConfirmation) {
    RoomData room;
    room.id = 7;
    room.name = "TestRoom";
    room.isPrivate = false;
    Packet p(MessageType::CREATE_ROOM_COMM, 1, 0, room);

    QSignalSpy spy(nm, &NetworkManager::RoomRequestConfirmation);
    dispatchPacket(p);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(NetworkManagerTest, DispatchPacket_JoinRoomComm_EmitsRoomRequestConfirmation) {
    RoomData room;
    room.id = 8;
    room.name = "AnotherRoom";
    Packet p(MessageType::JOIN_ROOM_COMM, 2, 0, room);

    QSignalSpy spy(nm, &NetworkManager::RoomRequestConfirmation);
    dispatchPacket(p);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(NetworkManagerTest, DispatchPacket_LogoutRequest_EmitsLogoutResultReceived) {
    Packet p(MessageType::LOGOUT_REQUEST, 1, 0, std::string("success"));

    QSignalSpy spy(nm, &NetworkManager::LogoutResultReceived);
    dispatchPacket(p);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(NetworkManagerTest, DispatchPacket_RoomInfoRequest_EmitsRoomInfoReceived) {
    RoomUserData data;
    data.id = 3;
    data.name = "InfoRoom";
    data.isPrivate = false;
    data.accessCode = 0;
    Packet p(MessageType::ROOM_INFO_REQUEST, 1, 0, data);

    QSignalSpy spy(nm, &NetworkManager::RoomInfoReceived);
    dispatchPacket(p);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(NetworkManagerTest, DispatchPacket_LeaveRoomRequest_EmitsLeaveResultReceivedWithCorrectIds) {
    LeaveRoomRequest req;
    req.roomId = 5;
    req.userId = 9;
    Packet p(MessageType::LEAVE_ROOM_REQUEST, 9, 5, req);

    QSignalSpy spy(nm, &NetworkManager::LeaveResultReceived);
    dispatchPacket(p);

    ASSERT_EQ(spy.count(), 1);
    EXPECT_EQ(spy.at(0).at(0).toUInt(), 5u);
    EXPECT_EQ(spy.at(0).at(1).toUInt(), 9u);
}

TEST_F(NetworkManagerTest, DispatchPacket_LoadMessRequest_EmitsMessagesReceived) {
    std::vector<MessageData> messages(3);
    Packet p(MessageType::LOAD_MESS_REQUEST, 1, 0, messages);

    QSignalSpy spy(nm, &NetworkManager::MessagesReceived);
    dispatchPacket(p);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(NetworkManagerTest, DispatchPacket_GenCodeRequest_EmitsAccessCodeReceivedWithCorrectValue) {
    uint32_t code = 54321u;
    Packet p(MessageType::GEN_CODE_REQUEST, 1, 0, code);

    QSignalSpy spy(nm, &NetworkManager::AccessCodeReceived);
    dispatchPacket(p);

    ASSERT_EQ(spy.count(), 1);
    EXPECT_EQ(spy.at(0).at(0).toUInt(), 54321u);
}

TEST_F(NetworkManagerTest, DispatchPacket_AccessCodeRequired_EmitsAccessCodeRequired) {
    JoinRoomRequest req;
    req.name = "PrivRoom";
    req.token = 0;
    Packet p(MessageType::ACCESS_CODE_REQUIRED, 1, 0, req);

    QSignalSpy spy(nm, &NetworkManager::AccessCodeRequired);
    dispatchPacket(p);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(NetworkManagerTest, DispatchPacket_FindUserRequest_EmitsUserFoundResult) {
    UserData user;
    user.id = 33;
    user.nickname = "foundUser";
    Packet p(MessageType::FIND_USER_REQUEST, 1, 0, user);

    QSignalSpy spy(nm, &NetworkManager::UserFoundResult);
    dispatchPacket(p);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(NetworkManagerTest, DispatchPacket_ErrorResponse_NoSignalEmitted) {
    Packet p(MessageType::ERROR_RESPONSE, 1, 0, std::string("some error"));

    QSignalSpy spyAuth(nm, &NetworkManager::AuthResultReceived);
    QSignalSpy spyMsg(nm, &NetworkManager::MessageReceived);
    dispatchPacket(p);

    EXPECT_EQ(spyAuth.count(), 0);
    EXPECT_EQ(spyMsg.count(), 0);
}
