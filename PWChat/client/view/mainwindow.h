#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common/RoomData.h"
#include "common/User.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void appendMessage(const QString& sender, const QString& text, bool isFromOthers=true);
    void appendUserRoomWidget(const QString& name, bool isRoom = false);
    void afterLoginChanges(const std::string& nickname, const std::vector<RoomData> userRooms);
    void addRoom(const RoomData& room);

private:
    Ui::MainWindow *ui;
    std::vector<RoomData> m_userRooms;
    std::vector<User> m_recentUsers;
    void scrollToBottom();
    QWidget* createMessageWidget(const QString& senderId, const QString& message, bool isFromOthers=true);
    QWidget* createUserRoomWidget(const QString& name, bool isRoom = false);

signals:
    void sendRequested(uint32_t targetId, std::string message, bool toRoom);
    void createRoomRequested(std::string roomName, bool isPrivate, bool isAdmin);
    void joinRoomRequested(std::string roomName);
    void audioRecordingStarted();
    void audioRecordingStopped();

private slots:
    void on_btnSend_clicked();
    void on_btnCreateRoom_clicked();
    void on_btnJoinRoom_clicked();
    void on_btnRecordAudio_pressed();
    void on_btnRecordAudio_released();
};

#endif // MAINWINDOW_H
