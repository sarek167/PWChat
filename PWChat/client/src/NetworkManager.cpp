#include "client/NetworkManager.h"
#include <iostream>
#include "common/Packet.h"
NetworkManager::~NetworkManager() {
    if (m_socket.is_open()) {
        asio::error_code ec;
        m_socket.close(ec);
    }
    m_io_context.stop();
}

void NetworkManager::connect(const std::string& host, const std::string& port) {
    try {
        asio::connect(m_socket, m_resolver.resolve(host, port));

        std::string data{"some client data..."};
        std::vector<char> body(data.begin(), data.end());

        Packet packetData(0, body);
        std::vector<char> toSend = packetData.pack();

        size_t bytesSent = asio::write(m_socket, asio::buffer(toSend));

        std::cout << "Wyslano pakiet!" << std::endl;
        std::cout << "Rozmiar danych: " << data.length() << " bajtow" << std::endl;
        std::cout << "Calkowity rozmiar (z naglowkiem): " << bytesSent << " bajtow" << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Blad podczas testu: " << e.what() << std::endl;
    }
}
