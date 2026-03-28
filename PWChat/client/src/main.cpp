#include <QCoreApplication>
#include <asio.hpp>
#include <string>
#include "client/NetworkManager.h"

using asio::ip::tcp;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::string data{"some client data..."};
    std::vector<char> body(data.begin(), data.end());

    Packet packetData(MessageType::TEXT_TO_USER, 0, 0, body);
    NetworkManager networkManager;
    networkManager.connect("127.0.0.1", "25000");
    networkManager.send(packetData);

    return a.exec();
}
