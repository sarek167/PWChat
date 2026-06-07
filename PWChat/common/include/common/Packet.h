#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <stdint.h>
#include <sstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include "PacketHeader.h"
#include "MessageType.h"

/**
 * @class Packet
 * @brief Class representing a full network packet that holds both a header and data (body).
 * * It uses the Cereal library to easily change structures/objects into raw bytes
 * ready to send over the network, and vice versa.
 */
class Packet {
public:
    /**
     * @brief Default constructor for Packet class creating an empty packet.
     */
    Packet() = default;

    /**
     * @brief Constructor that creates a packet from an existing header and a ready, raw binary body.
     * @param header The PacketHeader object with packet details.
     * @param body Vector containing the raw bytes of the body.
     */
    Packet(const PacketHeader& header, const std::vector<char>& body);

    /**
     * @brief Merges the header and the body into one flat byte array, ready to be sent via TCP socket.
     * @return std::vector<char> A buffer where the header bytes are followed right away by the body bytes.
     */
    std::vector<char> pack() const;

    /**
     * @brief Getter for the packet header.
     * @return const PacketHeader& Reference to the packet's header structure.
     */
    const PacketHeader& header() const;

    /**
     * @brief Getter for the raw body bytes.
     * @return const std::vector<char>& Reference to the vector holding the body data.
     */
    const std::vector<char>& body() const;

    friend class cereal::access; /**< Gives the Cereal library access to private fields so it can serialize them. */

    /**
     * @brief Serialization method used by Cereal to pack or unpack the packet fields.
     * @tparam Archive Type of the data stream (like binary or JSON).
     * @param archive Reference to the archive object doing the saving or loading.
     */
    template<class Archive>
    void serialize(Archive& archive) {
        archive(m_header, m_body);
    };

    /**
     * @brief Helper method that converts the raw body bytes back into a real C++ object/struct.
     * @tparam T The target struct or class type we want to unpack the data into.
     * @return T The fully reconstructed data object.
     */
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

    /**
     * @brief Template constructor that automatically creates the header and converts any given struct into the body buffer.
     * @tparam T The type of the data structure being sent.
     * @param type The type/opcode of the message.
     * @param targetId ID of the destination room or user.
     * @param senderId ID of the user sending this packet.
     * @param data The actual struct/object data we want to serialize and send.
     */
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
    PacketHeader m_header;    /**< Internal variable holding the packet header details. */
    std::vector<char> m_body; /**< Internal buffer holding the serialized body bytes. */
};

#endif // PACKET_H
