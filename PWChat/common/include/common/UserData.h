#ifndef USERDATA_H
#define USERDATA_H

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <stdint.h>
#include <string>

struct UserData {
    uint32_t id;
    std::string nickname;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(id, nickname);
    }
};


#endif // USERDATA_H
