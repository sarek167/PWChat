#ifndef ROOMUSERDATA_H
#define ROOMUSERDATA_H

#include "common/UserData.h"
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <stdint.h>
#include <string>

struct RoomUserData {
    uint32_t id;
    std::string name;
    bool isPrivate;
    uint32_t accessCode = 0;
    std::vector<UserData> users;
    std::vector<UserData> admins;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(id, name, isPrivate, accessCode, users, admins);
    }
};

#endif // ROOMUSERDATA_H
