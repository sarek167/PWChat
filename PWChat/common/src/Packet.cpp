#include "common/Packet.h"
#include <sstream>

Packet::Packet(const PacketHeader& header, const std::vector<char> body){
    m_header = header;
    m_body = body;
}


Packet::Packet(MessageType type, uint32_t targetId, uint32_t senderId, const std::vector<char>& body) {
    m_header.type = type;
    m_header.targetId = targetId;
    m_header.senderId = senderId;
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
        oarchive(m_body);
    }

    std::string serializedBody = ss.str();

    PacketHeader headerCopy = m_header;
    headerCopy.bodySize = static_cast<uint32_t>(serializedBody.size());

    std::vector<char> totalPacket;
    totalPacket.reserve(sizeof(PacketHeader) + headerCopy.bodySize);

    auto const headerPtr = reinterpret_cast<const char*>(&headerCopy);
    totalPacket.insert(totalPacket.end(), headerPtr, headerPtr + sizeof(PacketHeader));

    totalPacket.insert(totalPacket.end(), serializedBody.begin(), serializedBody.end());

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
