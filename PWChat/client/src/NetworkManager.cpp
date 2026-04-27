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
    auto toSend = std::make_shared<std::vector<char>>(p.pack());
    std::cout << p.header().targetId << " " << int(p.header().type) << std::endl;
    asio::post(m_io_context, [this, toSend]() {
        asio::async_write(m_socket, asio::buffer(*toSend),
                [toSend](std::error_code ec, std::size_t bytesTransferred) {
            if (!ec) {
                std::cout << "Wysłano pakiet długości " << bytesTransferred << std::endl;

            } else {
                std::cerr << "Błąd wysyłania: " << ec.message() << std::endl;
            }
        });
    });
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
        std::vector<char> rawBody(header.bodySize);
        auto bufs = m_buffer.data();
        std::copy(asio::buffers_begin(bufs), asio::buffers_begin(bufs)+header.bodySize, rawBody.begin());
        m_buffer.consume(header.bodySize);

        Packet packet(header, rawBody);


        if (header.type == MessageType::LOGIN_REQUEST) {
            try {
                std::vector<RoomData> rooms = packet.unpackBody<std::vector<RoomData>>();
                emit AuthResultReceived(packet.header().targetId, rooms);
            } catch (...) {
                std::cerr << "Błąd dekodowania auth" << std::endl;
            }

        } else if (header.type == MessageType::REGISTER_REQUEST) {
            try {
                RegisterRequest req = packet.unpackBody<RegisterRequest>();
                setUser(std::make_shared<User>(User(req.id, req.nickname)));
                emit RegisterResultReceived(req);
            } catch (...) {
                std::cerr << "Error while decoding body of register request" << std::endl;
            }
        } else if (header.type == MessageType::TEXT_TO_USER || header.type == MessageType::TEXT_TO_ROOM) {
            try {
                std::string message = packet.unpackBody<std::string>();
                emit MessageReceived(header.senderId, header.targetId, QString::fromStdString(message), header.type == MessageType::TEXT_TO_ROOM);
            } catch (...) {
                std::cerr << "Błąd dekodowania message" << std::endl;
            }
        } else if (header.type == MessageType::AUDIO_TO_USER) {
            try {
                emit AudioMessageReceived(QString::number(header.senderId), packet.body());
            } catch (...) {
                std::cerr << "Błąd dekodowania audio" << std::endl;
            }
        } else if (header.type == MessageType::CREATE_ROOM_COMM || header.type == MessageType::JOIN_ROOM_COMM) {
            RoomData room = packet.unpackBody<RoomData>();
            emit RoomRequestConfirmation(room);
        } else if (header.type == MessageType::ERROR_RESPONSE) {
            std::string message = packet.unpackBody<std::string>();
            std::cerr << "Error: " << message << std::endl;
        } else if (header.type == MessageType::LOGOUT_REQUEST) {
            emit LogoutResultReceived();
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

