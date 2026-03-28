#include "common/User.h"

User::User(uint32_t id, std::string nickname) {
    m_id = id;
    m_nickname = nickname;
}

const uint32_t User::id() const {
    return m_id;
}

const std::string User::nickname() const {
    return m_nickname;
}

void User::setId(uint32_t id) {
    m_id = id;
}

void User::setNickname(std::string nickname) {
    m_nickname = nickname;
}
