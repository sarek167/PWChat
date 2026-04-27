#include "client/AppManager.h"
#include "common/CreateRoomRequest.h"
#include "common/LoginRequest.h"
#include "common/RegisterRequest.h"

AppManager::AppManager(QObject *parent)
    : QObject(parent)
    , m_networkManager(new NetworkManager())
    , m_loginWin(nullptr)
    , m_mainWin(nullptr)
    , m_audioManager(new AudioManager()){
    setupConnections();
}

void AppManager::start() {
    m_networkManager->connect("127.0.0.1", "25000");
    m_loginWin.show();
}


void AppManager::setupConnections() {
    connect(&m_loginWin, &LoginWindow::loginRequested, this, [this](std::string nickname, std::string password){
        LoginRequest req{0, nickname, password};
        Packet loginPacket(MessageType::LOGIN_REQUEST, 0, 0, req);
        m_networkManager->send(loginPacket);
        m_networkManager->setUser(std::make_shared<User>(0, nickname));
    });

    connect(&m_loginWin, &LoginWindow::registerRequested, this, [this](std::string nickname, std::string password) {
        RegisterRequest req{0, nickname, password};
        Packet registerPacket(MessageType::REGISTER_REQUEST, 0, 0, req);
        m_networkManager->send(registerPacket);
    });

    connect(m_networkManager, &NetworkManager::AuthResultReceived, this, [this](uint32_t userId, const std::vector<RoomData>& rooms) {
        m_networkManager->user()->setId(userId);
        m_mainWin.afterLoginChanges(m_networkManager->user()->nickname(), rooms);
        m_loginWin.hide();
        m_loginWin.resetForms();
        m_mainWin.show();
    }, Qt::QueuedConnection);

    connect(m_networkManager, &NetworkManager::RoomRequestConfirmation, this, [this](const RoomData& room) {
        m_mainWin.addRoom(room);
    });

    connect(m_networkManager, &NetworkManager::MessageReceived, this, [this](const uint32_t senderId, const uint32_t targetId, const QString& message, bool toRoom) {
        m_mainWin.onMessageReceived(senderId, targetId, message, toRoom);
    });

    connect(m_networkManager, &NetworkManager::AudioMessageReceived, this, [this](const QString& senderId, const std::vector<char>& audioMessage) {
        std::vector<float> decodedAudio = m_audioManager->codec()->decode(audioMessage);
        m_audioManager->playAudio(decodedAudio);
    });

    connect(m_networkManager, &NetworkManager::LogoutResultReceived, this, [this] {
        m_networkManager->setUser(nullptr);
        m_mainWin.hide();
        m_loginWin.show();
    });

    connect(m_networkManager, &NetworkManager::RegisterResultReceived, this, [this](const RegisterRequest& req) {
        m_loginWin.resetForms();
    });

    connect(&m_mainWin, &MainWindow::sendRequested, this, [this](uint32_t targetId, std::string message, bool toRoom) {
        MessageType messType;
        std::cout << "In sendRequested signal" << std::endl;
        if (toRoom) {
            messType = MessageType::TEXT_TO_ROOM;
        } else {
            messType = MessageType::TEXT_TO_USER;
        }
        Packet sendPacket(messType, targetId, m_networkManager->user()->id(), message);
        m_networkManager->send(sendPacket);
    });

    connect(&m_mainWin, &MainWindow::createRoomRequested, this, [this](std::string roomName, bool isPrivate, bool isAdmin) {
        CreateRoomRequest request{roomName, isPrivate, isAdmin};

        Packet sendPacket(MessageType::CREATE_ROOM_COMM, 0, m_networkManager->user()->id(), request);
        m_networkManager->send(sendPacket);
    });

    connect(&m_mainWin, &MainWindow::joinRoomRequested, this, [this](std::string roomName) {
        Packet sendPacket(MessageType::JOIN_ROOM_COMM, 0, m_networkManager->user()->id(), roomName);
        m_networkManager->send(sendPacket);
    });

    connect(&m_mainWin, &MainWindow::audioRecordingStarted, this, [this] {
        m_audioManager->startRecording();
    });

    connect(&m_mainWin, &MainWindow::audioRecordingStopped, this, [this] {
        m_audioManager->stopRecording();
    });

    connect(&m_mainWin, &MainWindow::logoutRequested, this, [this] {
        Packet logoutPacket(MessageType::LOGOUT_REQUEST, 0, m_networkManager->user()->id(), NULL);
        m_networkManager->send(logoutPacket);
    });

    connect(m_audioManager, &AudioManager::audioReadyToSend, this, [this](const std::vector<char>& compressedData) {
        PacketHeader header;
        header.type = MessageType::AUDIO_TO_USER;
        header.senderId = m_networkManager->user()->id();
        header.targetId = 101;
        header.bodySize = static_cast<uint32_t>(compressedData.size());
        Packet sendPacket(header, compressedData);
        m_networkManager->send(sendPacket);
        std::cout << "Sent package with audio" << std::endl;
    });

}
