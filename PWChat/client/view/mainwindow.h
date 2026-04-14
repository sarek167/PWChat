#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client/NetworkManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(NetworkManager* manager, QWidget *parent = nullptr);
    ~MainWindow();
    void appendMessage(const QString& sender, const QString& text);


private:
    Ui::MainWindow *ui;
    uint32_t m_targetId;
    std::string m_message;
    bool m_toRoom;
    NetworkManager* m_networkManager;
    void scrollToBottom();
    QWidget* createMessageWidget(const QString& senderId, const QString& message);

signals:
    void sendRequested(uint32_t targetId, std::string message, bool toRoom);
    void createRoomRequested(std::string roomName, bool isPrivate, bool isAdmin);
    void joinRoomRequested(std::string roomName);

private slots:
    void on_btnSend_clicked();
    void on_btnCreateRoom_clicked();
    void on_btnJoinRoom_clicked();
};

#endif // MAINWINDOW_H
