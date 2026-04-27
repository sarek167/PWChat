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
    void resetForms();

private:
    uint32_t m_id;
    std::string m_nickname;
    Ui::LoginWindow *ui;

signals:
    void loginRequested(std::string nickname, std::string password);
    void registerRequested(std::string nickname, std::string password);
    void registerError();

private slots:
    void on_btnLogin_clicked();
    void on_btnRegister_clicked();
};

#endif // LOGINWINDOW_H
