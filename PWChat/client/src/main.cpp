#include <QCoreApplication>
#include <asio.hpp>
#include <string>
#include "client/NetworkManager.h"
#include <thread>
#include <chrono>

using asio::ip::tcp;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::string data{"some client data..."};
    std::vector<char> body(data.begin(), data.end());


    NetworkManager networkManager;
    networkManager.connect("127.0.0.1", "25000");

    Packet loginPacket(MessageType::LOGIN_REQUEST, 0, 101, body);
    networkManager.send(loginPacket);

    Packet loginPacketSecond(MessageType::LOGIN_REQUEST, 0, 100, body);
    networkManager.send(loginPacketSecond);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Packet packetData(MessageType::TEXT_TO_USER, 100, 101, body);
    networkManager.send(packetData);

    return a.exec();
}
