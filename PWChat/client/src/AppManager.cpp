#include "client/AppManager.h"
#include "common/RoomRequest.h"
#include "common/LoginRequest.h"

AppManager::AppManager(QObject *parent)
    : QObject(parent)
    , m_networkManager(new NetworkManager())
    , m_loginWin(m_networkManager, nullptr)
    , m_mainWin(m_networkManager, nullptr) {
    setupConnections();
}

void AppManager::start() {
    m_networkManager->connect("127.0.0.1", "25000");
    m_loginWin.show();
}


void AppManager::setupConnections() {
    connect(&m_loginWin, &LoginWindow::loginRequested, this, [this](uint32_t id, std::string nickname){
        LoginRequest req{id, nickname};
        Packet loginPacket(MessageType::LOGIN_REQUEST, 0, id, req);
        m_networkManager->send(loginPacket);
        m_networkManager->setUser(std::make_shared<User>(id, nickname));
    });

    connect(m_networkManager, &NetworkManager::AuthResultReceived, this, [this](std::string status) {
        if (status == "success") {
            std::cout << "Logged in successfuly" << std::endl;
            m_loginWin.hide();
            m_mainWin.show();
        }
    }, Qt::QueuedConnection);

    connect(&m_mainWin, &MainWindow::sendRequested, this, [this](uint32_t targetId, std::string message, bool toRoom) {
        std::vector<char> body(message.begin(), message.end());
        MessageType messType;

        if (toRoom) {
            messType = MessageType::TEXT_TO_ROOM;
        } else {
            messType = MessageType::TEXT_TO_USER;
        }
        Packet sendPacket(messType, targetId, m_networkManager->user()->id(), body);
        m_networkManager->send(sendPacket);
    });

    connect(&m_mainWin, &MainWindow::createRoomRequested, this, [this](std::string roomName, bool isPrivate, bool isAdmin) {
        RoomRequest request{roomName, isPrivate, isAdmin};

        Packet sendPacket(MessageType::CREATE_ROOM_COMM, 0, m_networkManager->user()->id(), request);
        m_networkManager->send(sendPacket);
    });

}
