#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "common/RoomData.h"
#include "client/ChatContext.h"
#include "common/UserData.h"
#include "common/MessageData.h"
#include "common/JoinRoomRequest.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    ChatContext currentChat();
    std::uint32_t userId();
    void onMessageReceived(const uint32_t senderId, const QString& senderName, const uint32_t targetId, const MessageContentType& msgType, const QString& text, bool toRoom);
    void displayOlderMessages(const std::vector<MessageData>& messages, const uint32_t userId);
    void appendMessage(const QString& sender, const MessageContentType& msgType, const QString& text, bool isFromOthers=true, bool addToTop = false, uint8_t topIndex = 0);
    void appendUserRoomWidget(const uint32_t id, const QString& name, bool isRoom = false);
    void appendUserWidget(const uint32_t id, const QString& name, bool isAdmin = false, bool amIAdmin=false);
    void afterLoginChanges(const std::uint32_t userId, const std::string& nickname, const std::vector<RoomData> userRooms);
    void addRoom(const RoomData& room);
    void onRoomWidgetClicked(uint32_t roomId);
    void displayRoomInfo(bool isPrivate, std::vector<UserData> users, std::vector<UserData> admins, bool amIAdmin=false, uint32_t accessCode = 0, bool isAdministered = true);
    void leaveRoom(const uint32_t roomId);
    void showContextMenu(const QPoint &pos, uint32_t userId);
    void scrollToBottom();
    void displayGeneratedCode(const QString& code);
    void requestCode(const std::string roomName);

private:
    Ui::MainWindow *ui;
    std::uint32_t m_userId;
    std::vector<RoomData> m_userRooms;
    std::vector<UserData> m_recentUsers;
    bool m_isWaitingForCode = false;
    std::string m_pendingRoomName;
    ChatContext m_currentChat;
    QWidget* createMessageWidget(const QString& senderId, const QString& message, bool isFromOthers=true);
    QWidget* createAudioMessageWidget(const QString& senderId, const QString& message, bool isFromOthers=true);
    QPushButton* createUserRoomWidget(const QString& name, bool isRoom = false);
    QPushButton* createUserWidget(const QString& name);
    void clearLayout(QLayout *layout, uint startingIdx=0);
    void resetJoinRoom();
    bool m_isLoadingHistory = false;

signals:
    void sendRequested(uint32_t targetId, std::string message, bool toRoom);
    void createRoomRequested(std::string roomName, bool isPrivate, bool isAdmin);
    void joinRoomRequested(std::string roomName, uint32_t code = 0);
    void audioRecordingStarted();
    void audioRecordingStopped();
    void logoutRequested();
    void roomInfoRequest(const uint32_t roomId);
    void leaveRoomRequested(const uint32_t roomId, const uint32_t userId = 0);
    void addAdminRequest(const uint32_t roomId, const uint32_t userId);
    void loadMessages(const uint32_t targetId, const uint32_t offset, bool fromRoom);
    void voicePlayRequested(const std::string& fileName, QPushButton* clickedButton);
    void generateCodeRequested(uint32_t roomId);

private slots:
    void on_btnSend_clicked();
    void on_btnCreateRoom_clicked();
    void on_btnJoinRoom_clicked();
    void on_btnRecordAudio_pressed();
    void on_btnRecordAudio_released();
    void on_btnLogout_clicked();
    void on_btnExit_clicked();
    void on_btnLeave_clicked();
    void on_btnGenerateCode_clicked();
};

#endif // MAINWINDOW_H
