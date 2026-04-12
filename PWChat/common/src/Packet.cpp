#include "common/Packet.h"
#include <sstream>

const std::vector<char>& Packet::body() const {
    return m_body;
}

const PacketHeader& Packet::header() const {
    return m_header;
}

std::vector<char> Packet::pack() const {
    PacketHeader headerCopy = m_header;

    std::vector<char> totalPacket;
    totalPacket.reserve(sizeof(PacketHeader) + headerCopy.bodySize);

    auto const headerPtr = reinterpret_cast<const char*>(&headerCopy);
    totalPacket.insert(totalPacket.end(), headerPtr, headerPtr + sizeof(PacketHeader));

    totalPacket.insert(totalPacket.end(), m_body.begin(), m_body.end());

    return totalPacket;
}

Packet Packet::unpack(const PacketHeader& header, std::vector<char>& rawData) {
    std::string dataStr(rawData.begin(), rawData.end());
    std::istringstream ss(dataStr, std::ios::binary);

    std::vector<char> deserializedBody;

    {
        cereal::BinaryInputArchive iarchive(ss);
        iarchive(deserializedBody);
    }
    Packet deserializedPacket(header, deserializedBody);
    return deserializedPacket;
}
