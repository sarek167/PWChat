#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "common/RoomData.h"
#include "client/ChatContext.h"
#include "common/UserData.h"

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
    void onMessageReceived(const uint32_t senderId, const uint32_t targetId, const QString& text, bool toRoom);
    void appendMessage(const QString& sender, const QString& text, bool isFromOthers=true);
    void appendUserRoomWidget(const uint32_t id, const QString& name, bool isRoom = false);
    void appendUserWidget(const uint32_t id, const QString& name, bool isAdmin = false);
    void afterLoginChanges(const std::string& nickname, const std::vector<RoomData> userRooms);
    void addRoom(const RoomData& room);
    void onRoomWidgetClicked(uint32_t roomId);
    void displayRoomInfo(bool isPrivate, std::vector<UserData> users, std::vector<UserData> admins);

private:
    Ui::MainWindow *ui;
    std::vector<RoomData> m_userRooms;
    std::vector<UserData> m_recentUsers;
    ChatContext m_currentChat;
    void scrollToBottom();
    QWidget* createMessageWidget(const QString& senderId, const QString& message, bool isFromOthers=true);
    QPushButton* createUserRoomWidget(const QString& name, bool isRoom = false);
    QPushButton* createUserWidget(const QString& name);
    void clearLayout(QLayout *layout, uint startingIdx=0);

signals:
    void sendRequested(uint32_t targetId, std::string message, bool toRoom);
    void createRoomRequested(std::string roomName, bool isPrivate, bool isAdmin);
    void joinRoomRequested(std::string roomName);
    void audioRecordingStarted();
    void audioRecordingStopped();
    void logoutRequested();
    void roomInfoRequest(const uint32_t roomId);

private slots:
    void on_btnSend_clicked();
    void on_btnCreateRoom_clicked();
    void on_btnJoinRoom_clicked();
    void on_btnRecordAudio_pressed();
    void on_btnRecordAudio_released();
    void on_btnLogout_clicked();
    void on_btnExit_clicked();
};

#endif // MAINWINDOW_H
