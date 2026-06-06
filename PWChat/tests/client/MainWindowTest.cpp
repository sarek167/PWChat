#include <gtest/gtest.h>
#include <QApplication>
#include <QSignalSpy>

#include "mainwindow.h"
#include "common/AuthResponse.h"
#include "common/RoomData.h"
#include "common/MessageData.h"
#include "common/UserData.h"
#include "client/ChatContext.h"

class MainWindowTest : public ::testing::Test {
protected:
    MainWindow* w = nullptr;

    void SetUp() override {
        w = new MainWindow();
    }

    void TearDown() override {
        delete w;
        w = nullptr;
    }

    AuthResponse makeAuthResponse(uint32_t id, const std::string& nickname,
                                  std::vector<RoomData> rooms = {},
                                  std::vector<UserData> chats = {}) {
        AuthResponse res;
        res.myId = id;
        res.myNickname = nickname;
        res.userRooms = rooms;
        res.userChats = chats;
        return res;
    }

    MessageData makeTextMessage(uint32_t senderId, uint32_t targetId,
                                const std::string& senderName, const std::string& text) {
        MessageData msg;
        msg.senderId = senderId;
        msg.targetId = targetId;
        msg.senderName = senderName;
        msg.message = text;
        msg.messageType = MessageContentType::TEXT;
        return msg;
    }
};


class MainWindowInitialStateTest : public MainWindowTest {};

TEST_F(MainWindowInitialStateTest, CurrentChat_BeforeLogin_IdIsZero) {
    EXPECT_EQ(w->currentChat().id, 0u);
}

TEST_F(MainWindowInitialStateTest, UserId_BeforeLogin_IsZero) {
    EXPECT_EQ(w->userId(), 0u);
}


class MainWindowAfterLoginTest : public MainWindowTest {};

TEST_F(MainWindowAfterLoginTest, AfterLoginChanges_SetsUserIdCorrectly) {
    w->afterLoginChanges(makeAuthResponse(42, "alice"));
    EXPECT_EQ(w->userId(), 42u);
}

TEST_F(MainWindowAfterLoginTest, AfterLoginChanges_WithRooms_AddsAllRooms) {
    RoomData r1; r1.id = 1; r1.name = "Room1";
    RoomData r2; r2.id = 2; r2.name = "Room2";
    QSignalSpy spy(w, &MainWindow::loadMessages);

    w->afterLoginChanges(makeAuthResponse(1, "alice", {r1, r2}));

    w->onChatWidgetClicked(1, true);
    EXPECT_EQ(w->currentChat().id, 1u);
}

TEST_F(MainWindowAfterLoginTest, AfterLoginChanges_CalledTwice_UserIdUpdatedToLatest) {
    w->afterLoginChanges(makeAuthResponse(1, "first"));
    w->afterLoginChanges(makeAuthResponse(99, "second"));
    EXPECT_EQ(w->userId(), 99u);
}


class MainWindowAddRoomTest : public MainWindowTest {};

TEST_F(MainWindowAddRoomTest, AddRoom_ValidRoom_RoomBecomesClickable) {
    w->afterLoginChanges(makeAuthResponse(5, "user"));
    RoomData room; room.id = 10; room.name = "NewRoom"; room.isPrivate = false;
    QSignalSpy spy(w, &MainWindow::loadMessages);

    w->addRoom(room);
    w->onChatWidgetClicked(10, true);

    EXPECT_EQ(w->currentChat().id, 10u);
}

TEST_F(MainWindowAddRoomTest, AddRoom_MultipleRooms_AllAccessible) {
    w->afterLoginChanges(makeAuthResponse(5, "user"));
    RoomData r1; r1.id = 1; r1.name = "A";
    RoomData r2; r2.id = 2; r2.name = "B";
    QSignalSpy spy(w, &MainWindow::loadMessages);

    w->addRoom(r1);
    w->addRoom(r2);

    w->onChatWidgetClicked(2, true);
    EXPECT_EQ(w->currentChat().id, 2u);
}


class MainWindowLeaveRoomTest : public MainWindowTest {};

TEST_F(MainWindowLeaveRoomTest, LeaveRoom_CurrentRoom_ResetsCurrentChat) {
    w->afterLoginChanges(makeAuthResponse(1, "user"));
    RoomData room; room.id = 7; room.name = "Room";
    w->addRoom(room);
    QSignalSpy spy(w, &MainWindow::loadMessages);
    w->onChatWidgetClicked(7, true);

    w->leaveRoom(7);

    EXPECT_EQ(w->currentChat().id, 0u);
}

TEST_F(MainWindowLeaveRoomTest, LeaveRoom_NonCurrentRoom_CurrentChatUnchanged) {
    w->afterLoginChanges(makeAuthResponse(1, "user"));
    RoomData r1; r1.id = 3; r1.name = "Stay";
    RoomData r2; r2.id = 4; r2.name = "Leave";
    w->addRoom(r1);
    w->addRoom(r2);
    QSignalSpy spy(w, &MainWindow::loadMessages);
    w->onChatWidgetClicked(3, true);

    w->leaveRoom(4);

    EXPECT_EQ(w->currentChat().id, 3u);
}

TEST_F(MainWindowLeaveRoomTest, LeaveRoom_NonExistentRoom_DoesNotCrash) {
    w->afterLoginChanges(makeAuthResponse(1, "user"));
    EXPECT_NO_FATAL_FAILURE(w->leaveRoom(999));
}


class MainWindowOnChatWidgetClickedTest : public MainWindowTest {};

TEST_F(MainWindowOnChatWidgetClickedTest, OnChatWidgetClicked_Room_EmitsLoadMessages) {
    w->afterLoginChanges(makeAuthResponse(1, "user"));
    RoomData room; room.id = 5; room.name = "R";
    w->addRoom(room);
    QSignalSpy spy(w, &MainWindow::loadMessages);

    w->onChatWidgetClicked(5, true);

    EXPECT_GE(spy.count(), 1);
    EXPECT_EQ(spy.last().at(0).toUInt(), 5u);
    EXPECT_EQ(spy.last().at(2).toBool(), true);
}

TEST_F(MainWindowOnChatWidgetClickedTest, OnChatWidgetClicked_User_EmitsLoadMessagesWithFromRoomFalse) {
    w->afterLoginChanges(makeAuthResponse(1, "user"));
    w->appendUserRoomWidget(8, "SomeUser", false);
    QSignalSpy spy(w, &MainWindow::loadMessages);

    w->onChatWidgetClicked(8, false);

    EXPECT_GE(spy.count(), 1);
    EXPECT_EQ(spy.last().at(2).toBool(), false);
}

TEST_F(MainWindowOnChatWidgetClickedTest, OnChatWidgetClicked_SameRoomTwice_EmitsRoomInfoRequest) {
    w->afterLoginChanges(makeAuthResponse(1, "user"));
    RoomData room; room.id = 6; room.name = "R";
    w->addRoom(room);
    QSignalSpy spyLoad(w, &MainWindow::loadMessages);
    QSignalSpy spyInfo(w, &MainWindow::roomInfoRequest);

    w->onChatWidgetClicked(6, true);
    w->onChatWidgetClicked(6, true);

    EXPECT_GE(spyInfo.count(), 1);
    EXPECT_EQ(spyInfo.last().at(0).toUInt(), 6u);
}

TEST_F(MainWindowOnChatWidgetClickedTest, OnChatWidgetClicked_UpdatesCurrentChatContext) {
    w->afterLoginChanges(makeAuthResponse(1, "user"));
    RoomData room; room.id = 11; room.name = "X";
    w->addRoom(room);
    QSignalSpy spy(w, &MainWindow::loadMessages);

    w->onChatWidgetClicked(11, true);

    EXPECT_EQ(w->currentChat().id, 11u);
    EXPECT_EQ(w->currentChat().type, ChatContext::Type::Room);
}


class MainWindowOnMessageReceivedTest : public MainWindowTest {};

TEST_F(MainWindowOnMessageReceivedTest, OnMessageReceived_MessageForCurrentRoom_DoesNotCrash) {
    w->afterLoginChanges(makeAuthResponse(1, "me"));
    RoomData room; room.id = 3; room.name = "R";
    w->addRoom(room);
    QSignalSpy spy(w, &MainWindow::loadMessages);
    w->onChatWidgetClicked(3, true);

    EXPECT_NO_FATAL_FAILURE(
        w->onMessageReceived(2, "alice", 3, MessageContentType::TEXT, "hello", true)
        );
}

TEST_F(MainWindowOnMessageReceivedTest, OnMessageReceived_MessageForDifferentRoom_DoesNotCrash) {
    w->afterLoginChanges(makeAuthResponse(1, "me"));
    RoomData r1; r1.id = 1; r1.name = "A";
    RoomData r2; r2.id = 2; r2.name = "B";
    w->addRoom(r1);
    w->addRoom(r2);
    QSignalSpy spy(w, &MainWindow::loadMessages);
    w->onChatWidgetClicked(1, true);

    EXPECT_NO_FATAL_FAILURE(
        w->onMessageReceived(9, "bob", 2, MessageContentType::TEXT, "hi", true)
        );
}

TEST_F(MainWindowOnMessageReceivedTest, OnMessageReceived_PrivateMessageFromCurrentPartner_DoesNotCrash) {
    w->afterLoginChanges(makeAuthResponse(1, "me"));
    w->appendUserRoomWidget(7, "partner", false);
    QSignalSpy spy(w, &MainWindow::loadMessages);
    w->onChatWidgetClicked(7, false);

    EXPECT_NO_FATAL_FAILURE(
        w->onMessageReceived(7, "partner", 1, MessageContentType::TEXT, "hey", false)
        );
}


class MainWindowDisplayOlderMessagesTest : public MainWindowTest {};

TEST_F(MainWindowDisplayOlderMessagesTest, DisplayOlderMessages_EmptyVector_DoesNotCrash) {
    w->afterLoginChanges(makeAuthResponse(1, "me"));
    EXPECT_NO_FATAL_FAILURE(w->displayOlderMessages({}, 1));
}

TEST_F(MainWindowDisplayOlderMessagesTest, DisplayOlderMessages_ValidMessages_DoesNotCrash) {
    w->afterLoginChanges(makeAuthResponse(1, "me"));
    RoomData room; room.id = 2; room.name = "R";
    w->addRoom(room);
    QSignalSpy spy(w, &MainWindow::loadMessages);
    w->onChatWidgetClicked(2, true);

    std::vector<MessageData> msgs = {
        makeTextMessage(2, 2, "alice", "msg1"),
        makeTextMessage(1, 2, "me",    "msg2"),
        makeTextMessage(2, 2, "alice", "msg3")
    };

    EXPECT_NO_FATAL_FAILURE(w->displayOlderMessages(msgs, 1));
}


class MainWindowRequestCodeTest : public MainWindowTest {};

TEST_F(MainWindowRequestCodeTest, RequestCode_SetsWaitingState_DoesNotCrash) {
    EXPECT_NO_FATAL_FAILURE(w->requestCode("PrivateRoom"));
}

TEST_F(MainWindowRequestCodeTest, RequestCode_ThenOnChatWidgetClicked_DoesNotCrash) {
    w->afterLoginChanges(makeAuthResponse(1, "user"));
    RoomData room; room.id = 5; room.name = "R";
    w->addRoom(room);
    QSignalSpy spy(w, &MainWindow::loadMessages);
    w->requestCode("PrivateRoom");

    EXPECT_NO_FATAL_FAILURE(w->onChatWidgetClicked(5, true));
}


class MainWindowDisplayRoomInfoTest : public MainWindowTest {};

TEST_F(MainWindowDisplayRoomInfoTest, DisplayRoomInfo_PublicRoom_DoesNotCrash) {
    w->afterLoginChanges(makeAuthResponse(1, "me"));
    UserData u1; u1.id = 2; u1.nickname = "alice";
    UserData u2; u2.id = 3; u2.nickname = "bob";

    EXPECT_NO_FATAL_FAILURE(
        w->displayRoomInfo(false, {u1, u2}, {}, false, 0, false)
        );
}

TEST_F(MainWindowDisplayRoomInfoTest, DisplayRoomInfo_PrivateRoomAsAdmin_DoesNotCrash) {
    w->afterLoginChanges(makeAuthResponse(1, "me"));
    UserData admin; admin.id = 1; admin.nickname = "me";

    EXPECT_NO_FATAL_FAILURE(
        w->displayRoomInfo(true, {}, {admin}, true, 12345, true)
        );
}

TEST_F(MainWindowDisplayRoomInfoTest, DisplayRoomInfo_CalledTwice_DoesNotCrash) {
    w->afterLoginChanges(makeAuthResponse(1, "me"));
    UserData u; u.id = 2; u.nickname = "alice";

    w->displayRoomInfo(false, {u}, {}, false);
    EXPECT_NO_FATAL_FAILURE(w->displayRoomInfo(false, {u}, {}, false));
}


class MainWindowDisplayGeneratedCodeTest : public MainWindowTest {};

TEST_F(MainWindowDisplayGeneratedCodeTest, DisplayGeneratedCode_ValidCode_DoesNotCrash) {
    w->afterLoginChanges(makeAuthResponse(1, "user"));
    RoomData room; room.id = 1; room.name = "R"; room.isPrivate = true;
    w->addRoom(room);
    QSignalSpy spy(w, &MainWindow::loadMessages);
    w->onChatWidgetClicked(1, true);
    w->displayRoomInfo(true, {}, {}, true, 0, false);

    EXPECT_NO_FATAL_FAILURE(w->displayGeneratedCode("54321"));
}
