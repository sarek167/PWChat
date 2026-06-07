#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

namespace Ui {
class LoginWindow;
}

/**
 * @class LoginWindow
 * @brief Class representing window for login.
 */
class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for LoginWindow class.
     * @param parent Pointer to the parent widget (defaults to nullptr).
     */
    explicit LoginWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor for LoginWindow class.
     */
    ~LoginWindow();

    /**
     * @brief Responsible for resetting all forms located in the window.
     */
    void resetForms();

private:
    Ui::LoginWindow *ui; /**< Pointer to LoginWindow UI */

signals:
    /**
     * @brief Signal sent after login form submit.
     * @param nickname The username or unique handle entered by the user.
     * @param password The plaintext account password credentials entered by the user.
     */
    void loginRequested(std::string nickname, std::string password);

    /**
     * @brief Signal sent after register form submit.
     * @param nickname The desired username or nickname handle chosen for registration.
     * @param password The baseline security password chosen for the new account profile.
     */
    void registerRequested(std::string nickname, std::string password);

    /**
     * @brief Signal sent when register form has errors - e.g. password and its confirmation do not match.
     */
    void registerError();

private slots:
    void on_btnLogin_clicked();
    void on_btnRegister_clicked();
};

#endif // LOGINWINDOW_H
