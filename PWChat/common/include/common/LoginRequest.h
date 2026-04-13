#ifndef LOGINREQUEST_H
#define LOGINREQUEST_H

#include <string>
#include <cereal/types/string.hpp>

struct LoginRequest {
    uint32_t userId;
    std::string nickname;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(userId, nickname);
    }
};


#endif // LOGINREQUEST_H
