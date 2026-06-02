#ifndef JOINROOMREQUEST_H
#define JOINROOMREQUEST_H

#include <cereal/types/string.hpp>
#include <string>

struct JoinRoomRequest {
    std::string name;
    uint32_t token = 0;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(name, token);
    }
};

#endif // JOINROOMREQUEST_H
