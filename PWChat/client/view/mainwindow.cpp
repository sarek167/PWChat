#include "mainwindow.h"
#include "ui/ui_mainwindow.h"
#include "createroomdialog.h"
#include <QTimer>
#include <QScrollBar>
#include <iostream>

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

void MainWindow::scrollToBottom() {
    QScrollBar *vScrollBar = ui->chatScroll->verticalScrollBar();

    QTimer::singleShot(50, [vScrollBar]() {
        vScrollBar->setValue(vScrollBar->minimum());
    });
}

QWidget* MainWindow::createMessageWidget(const QString& senderId, const QString& message, bool isFromOthers) {
    QWidget* container = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(container);

    QLabel* senderLabel = new QLabel(senderId);
    senderLabel->setStyleSheet(isFromOthers ? "font-weight: bold; color: #212121;" : "font-weight: bold; color: white;");

    QLabel* textLabel = new QLabel(message);
    textLabel->setWordWrap(true);
    textLabel->setStyleSheet(isFromOthers ? "color: #212121;" : "color: white;");

    layout->addWidget(senderLabel);
    layout->addWidget(textLabel);

    QString bgColor = isFromOthers ? "white" : "#212121";
    container->setStyleSheet(QString(
        "QWidget { "
        "   background-color: %1;"
        "   border-radius: 10px;"
        "   padding: 5px;"
        "   min-width: 100px;"
        "   max-width: 400px;"
        "}"
        ).arg(bgColor));
    return container;
}

void MainWindow::appendMessage(const QString& sender, const QString& text, bool isFromOthers) {
    QWidget* msgWidget = createMessageWidget(sender, text, isFromOthers);

    QWidget* wrapper = new QWidget();
    QHBoxLayout* wrapperLayout = new QHBoxLayout(wrapper);
    wrapperLayout->setContentsMargins(0,0,0,0);
    if (isFromOthers) {
        wrapperLayout->addWidget(msgWidget);
        wrapperLayout->addStretch();
    } else {
        wrapperLayout->addStretch();
        wrapperLayout->addWidget(msgWidget);
    }

    int spacerIdx = ui->verticalLayoutChat->count() - 1;
    ui->verticalLayoutChat->insertWidget(spacerIdx, wrapper);

    scrollToBottom();
}

QWidget* MainWindow::createUserRoomWidget(const QString& name, bool isRoom) {
    QWidget* container = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(container);

    QLabel* iconLabel = new QLabel();
    QIcon icon(":/room-icon.svg");
    QPixmap iconPixmap = icon.pixmap(QSize(24,24));
    iconLabel->setPixmap(iconPixmap);

    QLabel* nameLabel = new QLabel(name);

    layout->addWidget(iconLabel);
    layout->addWidget(nameLabel);
    layout->addStretch();

    container->setStyleSheet(QString(
                                 "QWidget { "
                                 "   background-color: white;"
                                 "   border-radius: 10px;"
                                 "   padding: 5px;"
                                 "}"));
    container->setCursor(Qt::PointingHandCursor);
    return container;
}

void MainWindow::appendUserRoomWidget(const QString& name, bool isRoom) {
    QWidget* cardWidget = createUserRoomWidget(name, isRoom);

    if (isRoom) {
        ui->verticalLayoutRooms->insertWidget(0, cardWidget);
    } else {
        ui->verticalLayoutPeople->insertWidget(0, cardWidget);
    }
}


void MainWindow::afterLoginChanges(const std::string& nickname, const std::vector<RoomData> userRooms) {
    ui->labelUsername->setText(QString::fromStdString(nickname));
    m_userRooms = userRooms;
    for (auto& room : userRooms) {
        appendUserRoomWidget(QString::fromStdString(room.name), true);
    }
}

void MainWindow::addRoom(const RoomData& room) {
    m_userRooms.push_back(room);
    appendUserRoomWidget(QString::fromStdString(room.name), true);
}

void MainWindow::on_btnSend_clicked() {
    uint32_t targetId = ui->editTargetID->text().toUInt();
    std::string message = ui->editMess->text().toStdString();
    bool toRoom = ui->checkSendRoom->isChecked();

    emit sendRequested(targetId, message, toRoom);
    appendMessage(QString("You"), QString::fromStdString(message), false);
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


void MainWindow::on_btnRecordAudio_pressed()
{
    ui->btnRecordAudio->setText("Recording...");
    emit audioRecordingStarted();
    std::cout << "Recording started" << std::endl;
}


void MainWindow::on_btnRecordAudio_released()
{
    ui->btnRecordAudio->setText("Record");
    emit audioRecordingStopped();
    std::cout << "Recording stopped" << std::endl;
}

