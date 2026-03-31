#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <memory>
#include "common/Packet.h"

class Session;
class User;

class Room {
public:
    Room(uint32_t id, std::string name) : m_id(id), m_name(name) {}
    virtual ~Room() = default;

    uint32_t id();
    std::string name();
    void addClient(std::shared_ptr<Session> clientToAdd);
    void removeClient(std::shared_ptr<Session> clientToRemove);
    virtual bool canJoin(const std::string& token) = 0;
    void broadcast(const Packet& p);
    void addAdmin(std::shared_ptr<User> adminToAdd);
    void removeAdmin(std::shared_ptr<User> adminToRemove);
protected:
    uint32_t m_id;
    std::string m_name;
    std::vector<std::shared_ptr<Session>> m_clients;
    std::vector<std::shared_ptr<User>> m_admins;
};

#endif // ROOM_H
