#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createroomdialog.h"

MainWindow::MainWindow(NetworkManager* manager, QWidget *parent)
    : m_networkManager(manager)
    , QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSend_clicked() {
    m_targetId = ui->editTargetID->text().toUInt();
    m_message = ui->editMess->text().toStdString();
    m_toRoom = ui->checkSendRoom->isChecked();

    emit sendRequested(m_targetId, m_message, m_toRoom);
}

void MainWindow::on_btnCreateRoom_clicked()
{
    CreateRoomDialog dlg(this);
    connect(&dlg, &CreateRoomDialog::createRoomRequested, this, &MainWindow::createRoomRequested);
    dlg.exec();
}


void MainWindow::on_btnJoinRoom_clicked()
{
    std::string roomName = ui->searchBar->text().toStdString();
    emit joinRoomRequested(roomName);
}

