#include "mainwindow.h"
#include "ui/ui_mainwindow.h"
#include "createroomdialog.h"
#include <QTimer>
#include <QScrollBar>

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

void MainWindow::scrollToBottom() {
    QScrollBar *vScrollBar = ui->chatScroll->verticalScrollBar();

    QTimer::singleShot(50, [vScrollBar]() {
        vScrollBar->setValue(vScrollBar->minimum());
    });
}

QWidget* MainWindow::createMessageWidget(const QString& senderId, const QString& message) {
    QWidget* container = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(container);

    QLabel* senderLabel = new QLabel(senderId);
    senderLabel->setStyleSheet("font-weight: bold; color: white;");

    QLabel* textLabel = new QLabel(message);
    textLabel->setWordWrap(true);
    textLabel->setStyleSheet("color: white;");

    layout->addWidget(senderLabel);
    layout->addWidget(textLabel);

    container->setStyleSheet(
        "QWidget { "
        "   background-color: #212121;"
        "   border-radius: 10px;"
        "   padding: 5px;"
        "   max-width: 400px;"
        "}"
        );
    return container;
}

void MainWindow::appendMessage(const QString& sender, const QString& text) {
    QWidget* msgWidget = createMessageWidget(sender, text);

    int spacerIdx = ui->verticalLayoutChat->count() - 1;
    ui->verticalLayoutChat->insertWidget(spacerIdx, msgWidget);

    scrollToBottom();
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

