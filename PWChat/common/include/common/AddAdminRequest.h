#ifndef ADDADMINREQUEST_H
#define ADDADMINREQUEST_H

#include <stdint.h>

struct AddAdminRequest {
    uint32_t roomId;
    uint32_t userId;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(roomId, userId);
    }
};

#endif // ADDADMINREQUEST_H
