#ifndef PACKET_H
#define PACKET_H
#include <vector>
#include <stdint.h>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include "PacketHeader.h"
#include "MessageType.h"

class Packet {
public:
    Packet() = default;
    Packet(const PacketHeader& header, const std::vector<char>& body);

    std::vector<char> pack() const;
    // static Packet unpackBody(std::vector<char>& rawData);

    const PacketHeader& header() const;
    const std::vector<char>& body() const;

    friend class cereal::access;

    template<class Archive>
    void serialize(Archive& archive) {
        archive(m_header, m_body);
    };

    template<typename T>
    T unpackBody() const {
        std::string dataStr(m_body.begin(), m_body.end());
        std::istringstream ss(dataStr, std::ios::binary);
        T data;

        {
            cereal::BinaryInputArchive iarchive(ss);
            iarchive(data);
        }
        return data;
    }

    template<typename T>
    Packet(MessageType type, uint32_t targetId, uint32_t senderId, const T& data) {
        m_header.type = type;
        m_header.targetId = targetId;
        m_header.senderId = senderId;

        std::ostringstream ss(std::ios::binary);
        {
            cereal::BinaryOutputArchive oarchive(ss);
            oarchive(data);
        }

        std::string serializedData = ss.str();
        m_body.assign(serializedData.begin(), serializedData.end());

        m_header.bodySize = static_cast<uint32_t>(m_body.size());
    }

private:
    PacketHeader m_header;
    std::vector<char> m_body;

};

#endif // PACKET_H
