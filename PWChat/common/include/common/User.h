#ifndef USER_H
#define USER_H

#include <string>
#include <stdint.h>

class User {
public:
    User(uint32_t id, std::string nickname);
    const uint32_t id() const;
    const std::string nickname() const;
    void setId(uint32_t id);
    void setNickname(std::string nick);
private:
    uint32_t m_id;
    std::string m_nickname;
};

#endif // USER_H
