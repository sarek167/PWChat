#include <QCoreApplication>
#include <asio.hpp>
#include <string>
#include <iostream>

using asio::ip::tcp;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    asio::io_context io_context;

    tcp::socket socket(io_context);
    tcp::resolver resolver(io_context);

    asio::connect(socket, resolver.resolve("127.0.0.1", "25000"));

    std::string data{"some client data...\0"};
    auto result = asio::write(socket, asio::buffer(data));

    std::cout << "data sent:" << data.length() << '/' << result << std::endl;

    std::error_code ec;
    socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
    socket.close();

    return a.exec();
}
