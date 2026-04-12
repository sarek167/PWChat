#include <QApplication>
#include <asio.hpp>
#include "client/AppManager.h"

using asio::ip::tcp;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AppManager appManager;
    appManager.start();

    return a.exec();
}
