#include "server/PrivateRoom.h"

bool PrivateRoom::canJoin(const std::string& token) {
    if (token == m_accessCode) {
        return true;
    } else {
        return false;
    }
}
