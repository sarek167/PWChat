#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <memory>

class Session;
class User;

class Room {
public:
    virtual ~Room() = default;

    std::string name();
    void addClient(std::shared_ptr<Session> clientToAdd);
    void removeClient(std::shared_ptr<Session> clientToRemove);
    virtual bool canJoin(const std::string& token) = 0;
protected:
    std::string m_name;
    std::vector<std::shared_ptr<Session>> m_clients;
    std::vector<std::shared_ptr<User>> m_admins;
};

#endif // ROOM_H
