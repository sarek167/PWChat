#ifndef PACKETHEADER_H
#define PACKETHEADER_H

#include <stdint.h>

struct PacketHeader {
    uint32_t signature = 0x50574348;
    uint8_t type = 0;
    uint32_t bodySize = 0;

    template<class Archive>
    void serialize(Archive& archive) {
        archive(signature, type, bodySize);
    };
};

#endif // PACKETHEADER_H
