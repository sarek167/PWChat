#include "client/OpusCodec.h"
#include <opus.h>
#include <iostream>
#include <stdint.h>
#include <cstring>

OpusCodec::OpusCodec() {
    m_frameSize = 960;
    int error;
    m_encoder = opus_encoder_create(48000, 1, OPUS_APPLICATION_VOIP, &error);
    m_decoder = opus_decoder_create(48000, 1, &error);
    if (error != OPUS_OK) {
        std::cerr << "Error while initializing opus encoder and decoder" << std::endl;
    }
}

OpusCodec::~OpusCodec() {
    if (m_encoder) {
        opus_encoder_destroy(m_encoder);
        m_encoder = nullptr;
    }

    if (m_decoder) {
        opus_decoder_destroy(m_decoder);
        m_decoder = nullptr;
    }
}

std::vector<char> OpusCodec::encode(const std::vector<float>& pcmData) {
    std::vector<char> totalEncodedData;
    unsigned char outBuf[4000];

    for (size_t i = 0; i+m_frameSize <= pcmData.size(); i+= m_frameSize) {
        int nBytes = opus_encode_float(m_encoder, &pcmData[i], m_frameSize, outBuf, sizeof(outBuf));
        if (nBytes > 0) {
            uint16_t size = static_cast<uint16_t>(nBytes);
            totalEncodedData.insert(totalEncodedData.end(), reinterpret_cast<char*>(&size), reinterpret_cast<char*>(&size) + sizeof(size));
            totalEncodedData.insert(totalEncodedData.end(), outBuf, outBuf + nBytes);
        }
    }

    return totalEncodedData;
}

std::vector<float> OpusCodec::decode(const std::vector<char>& compressedData) {
    std::vector<float> totalDecodedData;
    size_t offset = 0;
    float outputBuffer[960];

    while (offset + sizeof(int16_t) < compressedData.size()) {
        uint16_t frameByteSize;
        memcpy(&frameByteSize, &compressedData[offset], sizeof(uint16_t));
        offset += sizeof(uint16_t);

        if (offset + frameByteSize > compressedData.size()) break;

        int samplesDecoded = opus_decode_float(
            m_decoder,
            reinterpret_cast<const unsigned char*>(&compressedData[offset]),
            frameByteSize,
            outputBuffer,
            m_frameSize,
            0);
        if (samplesDecoded > 0) {
            totalDecodedData.insert(totalDecodedData.end(), outputBuffer, outputBuffer + samplesDecoded);
        }

        offset += frameByteSize;
    }

    return totalDecodedData;
}
