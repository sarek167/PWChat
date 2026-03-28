#ifndef PACKET_H
#define PACKET_H
#include <vector>
#include <stdint.h>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include "PacketHeader.h"

class Packet {
public:
    Packet() = default;
    Packet(uint8_t type, const std::vector<char> body);

    std::vector<char> pack() const;
    static Packet unpack(std::vector<char>& rawData);

    const PacketHeader& header() const;
    const std::vector<char>& body() const;

    friend class cereal::access;

template<class Archive>
    void serialize(Archive& archive) {
    archive(m_header, m_body);
};

private:
    PacketHeader m_header;
    std::vector<char> m_body;
};

#endif // PACKET_H
