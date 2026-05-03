#ifndef LEAVEROOMREQUEST_H
#define LEAVEROOMREQUEST_H

#include <stdint.h>

struct LeaveRoomRequest {
    uint32_t roomId;
    uint32_t userId;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(roomId, userId);
    }
};

#endif // LEAVEROOMREQUEST_H
