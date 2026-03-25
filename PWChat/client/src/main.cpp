#include <QCoreApplication>
#include <asio.hpp>
#include <string>
#include "client/NetworkManager.h"

using asio::ip::tcp;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NetworkManager networkManager;
    networkManager.connect("127.0.0.1", "25000");

    return a.exec();
}
