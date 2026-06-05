#include <gtest/gtest.h>
#include "client/OpusCodec.h"
#include <vector>
#include <cmath>

class OpusCodecTest : public ::testing::Test {
protected:
    void SetUp() override {
        codec = std::make_unique<OpusCodec>();
    }

    std::vector<float> generateSineWave(size_t samples, float frequency, float sampleRate) {
        std::vector<float> pcm(samples);
        for (size_t i = 0; i < samples; ++i) {
            pcm[i] = std::sin(2.0f * M_PI * frequency * i / sampleRate);
        }
        return pcm;
    }

    std::unique_ptr<OpusCodec> codec;
};

TEST_F(OpusCodecTest, Constructor_Initialization_SucceedsWithoutExceptions) {
    EXPECT_NO_THROW({
        OpusCodec testCodec;
    });
}

TEST_F(OpusCodecTest, EncodeDecode_ValidPcmData_ReturnsDecodedDataWithMatchingSize) {
    std::vector<float> originalPcm = generateSineWave(960, 440.0f, 48000.0f);

    std::vector<char> compressed = codec->encode(originalPcm);
    ASSERT_FALSE(compressed.empty());

    std::vector<float> decodedPcm = codec->decode(compressed);
    EXPECT_EQ(decodedPcm.size(), originalPcm.size());
}

TEST_F(OpusCodecTest, Encode_InputSizeLessThanFrameSize_ReturnsEmptyVector) {
    std::vector<float> smallPcm(500, 0.5f);

    std::vector<char> compressed = codec->encode(smallPcm);
    EXPECT_TRUE(compressed.empty());
}

TEST_F(OpusCodecTest, Encode_EmptyInputVector_ReturnsEmptyVector) {
    std::vector<float> emptyPcm;

    std::vector<char> compressed = codec->encode(emptyPcm);
    EXPECT_TRUE(compressed.empty());
}

TEST_F(OpusCodecTest, Decode_EmptyInputVector_ReturnsEmptyVector) {
    std::vector<char> emptyCompressed;

    std::vector<float> decoded = codec->decode(emptyCompressed);
    EXPECT_TRUE(decoded.empty());
}

TEST_F(OpusCodecTest, Decode_InvalidFrameSizeHeader_AbortsLoopSafelyAndReturnsEmpty) {
    std::vector<char> corruptedData = {0x00, 0x20, 0x01, 0x02};

    std::vector<float> decoded = codec->decode(corruptedData);
    EXPECT_TRUE(decoded.empty());
}

TEST_F(OpusCodecTest, Decode_HugeFrameSizeHeader_TriggersBoundaryGuardAndReturnsEmpty) {
    std::vector<char> maliciousData = {0x10, 0x27, 0x01, 0x02};

    std::vector<float> decoded = codec->decode(maliciousData);
    EXPECT_TRUE(decoded.empty());
}

TEST_F(OpusCodecTest, Encode_MultipleFrames_SucceedsAndPacksCorrectly) {
    std::vector<float> originalPcm = generateSineWave(960 * 3, 440.0f, 48000.0f);

    std::vector<char> compressed = codec->encode(originalPcm);
    ASSERT_FALSE(compressed.empty());

    std::vector<float> decodedPcm = codec->decode(compressed);
    EXPECT_EQ(decodedPcm.size(), 960 * 3);
}

TEST_F(OpusCodecTest, Decode_IncompletePacketPayload_AbortsAtRangeCheck) {
    std::vector<char> incompleteData = {0x05, 0x00, 0x01, 0x02};

    std::vector<float> decoded = codec->decode(incompleteData);
    EXPECT_TRUE(decoded.empty());
}
