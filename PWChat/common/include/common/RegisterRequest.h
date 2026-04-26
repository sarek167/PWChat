#ifndef REGISTERREQUEST_H
#define REGISTERREQUEST_H

#include <string>
#include <cereal/types/string.hpp>

struct RegisterRequest {
    uint32_t id;
    std::string nickname;
    std::string password;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(id, nickname, password);
    }
};

#endif // REGISTERREQUEST_H
