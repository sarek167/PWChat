#include "loginwindow.h"
#include "ui/ui_LoginWindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->labelGoToRegister, &QLabel::linkActivated, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
        this->setWindowTitle("Register");
    });

    connect(ui->labelGoToLogin, &QLabel::linkActivated, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
        this->setWindowTitle("Login");
    });

}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::on_btnLogin_clicked() {
    std::string nickname = ui->editNick->text().toStdString();
    std::string password = ui->editPasswordL->text().toStdString();

    emit loginRequested(nickname, password);
    ui->btnLogin->setEnabled(false);
    ui->btnLogin->setText("Logging in...");
}


void LoginWindow::on_btnRegister_clicked() {
    std::string nick = ui->editNickR->text().toStdString();
    std::string pass = ui->editPass->text().toStdString();
    std::string passConf = ui->editConfPass->text().toStdString();

    if (pass != passConf) {
        emit registerError();
        return;
    }

    emit registerRequested(nick, pass);
    ui->btnRegister->setEnabled(false);
    ui->btnRegister->setText("Registering...");
}
