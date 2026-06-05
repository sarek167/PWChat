#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "common/Packet.h"

struct TestSerializableData {
    uint32_t id;
    std::string name;

    template<class Archive>
    void serialize(Archive& archive) {
        archive(id, name);
    }
};

static PacketHeader CreateTestHeader(MessageType type, uint32_t targetId, uint32_t senderId, uint32_t bodySize) {
    PacketHeader header;
    header.type = type;
    header.targetId = targetId;
    header.senderId = senderId;
    header.bodySize = bodySize;
    return header;
}

TEST(PacketTest, ConstructorWithHeaderAndBody_ValidInputs_StoresPropertiesCorrectly) {
    PacketHeader expectedHeader = CreateTestHeader(MessageType::MESS_TO_USER, 100, 200, 5);
    std::vector<char> expectedBody{'h', 'e', 'l', 'l', 'o'};

    Packet packet(expectedHeader, expectedBody);

    EXPECT_EQ(packet.header().type, expectedHeader.type);
    EXPECT_EQ(packet.header().targetId, expectedHeader.targetId);
    EXPECT_EQ(packet.header().senderId, expectedHeader.senderId);
    EXPECT_EQ(packet.header().bodySize, expectedHeader.bodySize);
    EXPECT_EQ(packet.body(), expectedBody);
}

TEST(PacketTest, ConstructorWithHeaderAndBody_EmptyBody_StoresPropertiesCorrectly) {
    PacketHeader expectedHeader = CreateTestHeader(MessageType::LOGIN_REQUEST, 0, 0, 0);
    std::vector<char> expectedBody{};

    Packet packet(expectedHeader, expectedBody);

    EXPECT_TRUE(packet.body().empty());
    EXPECT_EQ(packet.header().bodySize, 0u);
}

TEST(PacketTest, TemplatedConstructor_ValidObject_SerializesBodyAndCalculatesSize) {
    TestSerializableData originalData{42, "John Doe"};

    Packet packet(MessageType::MESS_TO_ROOM, 99, 11, originalData);

    EXPECT_EQ(packet.header().type, MessageType::MESS_TO_ROOM);
    EXPECT_EQ(packet.header().targetId, 99u);
    EXPECT_EQ(packet.header().senderId, 11u);
    EXPECT_GT(packet.header().bodySize, 0u);
    EXPECT_FALSE(packet.body().empty());
    EXPECT_EQ(packet.header().bodySize, packet.body().size());
}

TEST(PacketTest, Pack_ValidPacket_ReturnsCorrectBinaryLayout) {
    PacketHeader header = CreateTestHeader(MessageType::MESS_TO_USER, 50, 60, 4);
    std::vector<char> body{'y', 'a', 'p', 's'};
    Packet packet(header, body);

    std::vector<char> result = packet.pack();

    size_t expectedTotalSize = sizeof(PacketHeader) + body.size();
    ASSERT_EQ(result.size(), expectedTotalSize);

    PacketHeader extractedHeader;
    memcpy(&extractedHeader, result.data(), sizeof(PacketHeader));
    EXPECT_EQ(extractedHeader.type, header.type);
    EXPECT_EQ(extractedHeader.targetId, header.targetId);
    EXPECT_EQ(extractedHeader.senderId, header.senderId);
    EXPECT_EQ(extractedHeader.bodySize, header.bodySize);

    std::vector<char> extractedBody(result.begin() + sizeof(PacketHeader), result.end());
    EXPECT_EQ(extractedBody, body);
}

TEST(PacketTest, Pack_EmptyBodyPacket_ReturnsOnlyHeaderBytes) {
    PacketHeader header = CreateTestHeader(MessageType::LOGOUT_REQUEST, 1, 2, 0);
    Packet packet(header, {});

    std::vector<char> result = packet.pack();

    ASSERT_EQ(result.size(), sizeof(PacketHeader));
}

TEST(PacketTest, UnpackBody_ValidSerializedData_ReturnsMatchingObject) {
    TestSerializableData originalData{1337, "SecureMessage"};
    Packet packet(MessageType::MESS_TO_USER, 1, 2, originalData);

    TestSerializableData unpackedData = packet.unpackBody<TestSerializableData>();

    EXPECT_EQ(unpackedData.id, originalData.id);
    EXPECT_EQ(unpackedData.name, originalData.name);
}

TEST(PacketTest, UnpackBody_CorruptedBinaryData_ThrowsException) {
    PacketHeader header = CreateTestHeader(MessageType::MESS_TO_USER, 1, 2, 3);
    std::vector<char> corruptedBody{'b', 'a', 'd'};
    Packet packet(header, corruptedBody);

    EXPECT_THROW(packet.unpackBody<TestSerializableData>(), cereal::Exception);
}

TEST(PacketTest, UnpackBody_EmptyBodyData_ThrowsException) {
    PacketHeader header = CreateTestHeader(MessageType::MESS_TO_USER, 1, 2, 0);
    Packet packet(header, {});

    EXPECT_THROW(packet.unpackBody<TestSerializableData>(), cereal::Exception);
}
