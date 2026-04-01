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

std::shared_ptr<User> NetworkManager::user() {
    return m_user;
}

void NetworkManager::setUser(std::shared_ptr<User> user) {
    m_user = user;
}

void NetworkManager::connect(const std::string& host, const std::string& port) {
    try {
        asio::connect(m_socket, m_resolver.resolve(host, port));
        std::thread([this]() {
            auto work_guard = asio::make_work_guard(m_io_context);
            m_io_context.run();
        }).detach();
        doRead();

    } catch (std::exception& e) {
        std::cerr << "Blad podczas testu: " << e.what() << std::endl;

    }
}

void NetworkManager::send(const Packet& p) {
    std::vector<char> toSend = p.pack();

    size_t bytesSent = asio::write(m_socket, asio::buffer(toSend));

    std::cout << "Wyslano pakiet!" << std::endl;
    std::cout << "Calkowity rozmiar (z naglowkiem): " << bytesSent << " bajtow" << std::endl;
}

void NetworkManager::doRead() {
    waitForRequest();
}

void NetworkManager::waitForRequest() {
    asio::async_read(m_socket, m_buffer, asio::transfer_exactly(sizeof(PacketHeader)),
         [this](std::error_code ec, std::size_t bytesTransferred) {
             if (!ec) {
                 PacketHeader header;
                 asio::buffer_copy(asio::buffer(&header, sizeof(PacketHeader)), m_buffer.data());
                 m_buffer.consume(bytesTransferred);


                 readBody(header);

             } else {
                 std::cout << "error: " << ec << std::endl;
             }
         });
}

void NetworkManager::readBody(PacketHeader header) {
    asio::async_read(m_socket, m_buffer, asio::transfer_exactly(header.bodySize),
     [this,header](std::error_code ec, std::size_t bytesTransferred) {
        std::istream is(&m_buffer);
        std::vector<char> deserializedBody;

        try {
            cereal::BinaryInputArchive iarchive(is);
            iarchive(deserializedBody);
        } catch (const std::exception& e) {
            std::cerr << "Cereal error: " << e.what() << std::endl;
        }

        Packet packet(header, deserializedBody);

        if (header.type == MessageType::AUTH_RESPONSE) {
            emit AuthResultReceived(std::string(deserializedBody.begin(), deserializedBody.end()));
        }
        std::cout << "KLIENT DOSTAŁ PAKIET!!!" << std::endl;
        std::cout << packet.header().signature << std::endl;
        std::cout << (int)packet.header().type << std::endl;
        std::cout << (int)packet.header().bodySize << std::endl;
        std::string message{packet.body().begin(), packet.body().end()};
        std::cout << message << std::endl;


        waitForRequest();
     });
}

