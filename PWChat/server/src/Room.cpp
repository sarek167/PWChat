#include "server/Room.h"
#include "server/Session.h"
#include <algorithm>
#include <vector>
#include <iostream>

uint32_t Room::id() {
    return m_id;
}

std::string Room::name() {
    return m_name;
}

uint32_t Room::ownerId() {
    return m_ownerId;
}

void Room::addClient(std::shared_ptr<Session> clientToAdd) {
    auto it = std::find(m_clients.begin(), m_clients.end(), clientToAdd);
    std::cout << "trying to add client :" << clientToAdd->userId() << std::endl;
    if (it != m_clients.end()) {
        std::cerr << "Error: User " << clientToAdd->userId() << " is already in room " << m_name << std::endl;
    } else {
        std::cout << "Adding client " << clientToAdd->userId() << " to room " <<m_name << std::endl;
        m_clients.push_back(clientToAdd);
    }
}

void Room::removeClient(std::shared_ptr<Session> clientToRemove) {
    auto it = std::find(m_clients.begin(), m_clients.end(), clientToRemove);

    if (it!= m_clients.end()) {
        m_clients.erase(it, m_clients.end());
    } else {
        std::cerr << "Error: User " << clientToRemove->userId() << " wasn't part of the room " << m_name << std::endl;
    }
}

void Room::broadcast(const Packet& p) {
    for (auto& client : m_clients) {
        if (client->userId() != p.header().senderId) {
            client->deliver(p);
        }
    }
}

void Room::addAdmin(uint32_t adminToAdd) {
    auto it = std::find(m_adminIds.begin(), m_adminIds.end(), adminToAdd);

    if (it != m_adminIds.end()) {
        std::cerr << "Error: User " << adminToAdd << " is already an admin of the room " << m_name << std::endl;
    } else {
        m_adminIds.push_back(adminToAdd);
    }
}

void Room::removeAdmin(uint32_t adminToRemove) {
    auto it = std::find(m_adminIds.begin(), m_adminIds.end(), adminToRemove);

    if (it != m_adminIds.end()) {
        m_adminIds.erase(it, m_adminIds.end());
    } else {
        std::cerr << "Error: User " << adminToRemove << " wasn't admin of the room " << m_name << std::endl;
    }
}
