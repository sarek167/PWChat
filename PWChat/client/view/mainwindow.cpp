#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
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
