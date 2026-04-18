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


void MainWindow::on_btnSend_clicked() {
    m_targetId = ui->editTargetID->text().toUInt();
    m_message = ui->editMess->text().toStdString();
    m_toRoom = ui->checkSendRoom->isChecked();

    emit sendRequested(m_targetId, m_message, m_toRoom);
    appendMessage(QString("You"), QString::fromStdString(m_message), false);
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

