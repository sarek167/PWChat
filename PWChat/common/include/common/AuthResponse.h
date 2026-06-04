#ifndef AUTHRESPONSE_H
#define AUTHRESPONSE_H

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <stdint.h>
#include <string>
#include "common/UserData.h"
#include "common/RoomData.h"

struct AuthResponse {
    uint32_t myId;
    std::string myNickname;
    std::vector<UserData> userChats;
    std::vector<RoomData> userRooms;


    template<class Archive>
    void serialize(Archive & archive) {
        archive(myId, myNickname, userChats, userRooms);
    }
};

#endif // AUTHRESPONSE_H
