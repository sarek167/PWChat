#ifndef MESSAGEREQUEST_H
#define MESSAGEREQUEST_H

#include <cereal/archives/binary.hpp>
#include <stdint.h>


struct MessageRequest {
    uint32_t targetId;
    uint32_t offset;
    bool fromRoom;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(targetId, offset, fromRoom);
    }
};


#endif // MESSAGEREQUEST_H
