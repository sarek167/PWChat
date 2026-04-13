#ifndef ROOMREQUEST_H
#define ROOMREQUEST_H
#include <string>
#include <cereal/types/string.hpp>

struct CreateRoomRequest {
    std::string roomName;
    bool isPrivate;
    bool isAdmin;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(roomName, isPrivate, isAdmin);
    }
};

#endif // ROOMREQUEST_H
