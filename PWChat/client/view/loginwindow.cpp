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
    m_id = ui->editID->text().toUInt();
    m_nickname = ui->editNick->text().toStdString();

    emit loginRequested(m_id, m_nickname);
    ui->btnLogin->setEnabled(false);
    ui->btnLogin->setText("Logging in...");
}
