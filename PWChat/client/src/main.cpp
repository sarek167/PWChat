#include <QApplication>
#include <asio.hpp>
#include "loginwindow.h"
#include <string>
#include "client/NetworkManager.h"
#include <chrono>

using asio::ip::tcp;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    NetworkManager networkManager;
    networkManager.connect("127.0.0.1", "25000");

    LoginWindow logW;

    logW.show();

    QObject::connect(&logW, &LoginWindow::loginRequested, [&](uint32_t id, std::string nickname){
        std::vector<char> body(nickname.begin(), nickname.end());
        Packet loginPacket(MessageType::LOGIN_REQUEST, 0, id, body);
        networkManager.send(loginPacket);
    });
    // std::string data{"some client data..."};
    // std::vector<char> body(data.begin(), data.end());


    // Packet loginPacket(MessageType::LOGIN_REQUEST, 0, 101, body);
    // networkManager.send(loginPacket);

    // Packet loginPacketSecond(MessageType::LOGIN_REQUEST, 0, 100, body);
    // networkManager.send(loginPacketSecond);

    // Packet loginPacketThird(MessageType::LOGIN_REQUEST, 0, 102, body);
    // networkManager.send(loginPacketThird);

    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // Packet packetData1(MessageType::TEXT_TO_USER, 101, 100, body);
    // networkManager.send(packetData1);

    // Packet packetData2(MessageType::TEXT_TO_USER, 102, 100, body);
    // networkManager.send(packetData2);


    // std::string dataR{"some client data TO THE ROOM..."};
    // std::vector<char> bodyR(data.begin(), data.end());
    // Packet packetDataR(MessageType::TEXT_TO_ROOM, 0, 101, bodyR);
    // networkManager.send(packetDataR);

    return a.exec();
}

