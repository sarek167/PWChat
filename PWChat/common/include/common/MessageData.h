#ifndef MESSAGEDATA_H
#define MESSAGEDATA_H

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <stdint.h>
#include <string>

struct MessageData {
    uint32_t senderId;
    uint32_t targetId;
    std::string message;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(senderId, targetId, message);
    }
};

#endif // MESSAGEDATA_H
