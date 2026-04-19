#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
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
