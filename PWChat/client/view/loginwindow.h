#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "client/NetworkManager.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private:
    uint32_t m_id;
    std::string m_nickname;
    Ui::LoginWindow *ui;

signals:
    void loginRequested(uint32_t id, std::string nickname);

private slots:
    void on_btnLogin_clicked();
};

#endif // LOGINWINDOW_H
