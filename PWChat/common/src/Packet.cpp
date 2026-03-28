#include "common/Packet.h"
#include <sstream>

Packet::Packet(uint8_t type, const std::vector<char> body) {
    m_header.type = type;
    m_header.bodySize = static_cast<uint32_t>(body.size());
    m_body = body;
}

const std::vector<char>& Packet::body() const {
    return m_body;
}

const PacketHeader& Packet::header() const {
    return m_header;
}

std::vector<char> Packet::pack() const {
    std::ostringstream ss(std::ios::binary);

    {
        cereal::BinaryOutputArchive oarchive(ss);
        oarchive(*this);
    }

    std::string str_ss = ss.str();
    return std::vector<char>(str_ss.begin(), str_ss.end());
}

Packet Packet::unpack(std::vector<char>& rawData) {
    std::string dataStr(rawData.begin(), rawData.end());
    std::istringstream ss(dataStr, std::ios::binary);

    Packet deserializedPacket;
    {
        cereal::BinaryInputArchive iarchive(ss);
        iarchive(deserializedPacket);
    }

    return deserializedPacket;
}
