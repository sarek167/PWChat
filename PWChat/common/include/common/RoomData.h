#ifndef ROOMDATA_H
#define ROOMDATA_H

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <stdint.h>
#include <string>

struct RoomData {
    uint32_t id;
    std::string name;
    bool isPrivate;
    uint32_t ownerId;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(id, name, isPrivate, ownerId);
    }
};

#endif // ROOMDATA_H
