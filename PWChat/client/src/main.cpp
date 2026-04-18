#include <QApplication>
#include <asio.hpp>
#include "client/AppManager.h"

using asio::ip::tcp;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<std::vector<char>>("std::vector<char>");
    AppManager appManager;
    appManager.start();

    return a.exec();
}
