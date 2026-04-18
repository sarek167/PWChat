#ifndef OPUSCODEC_H
#define OPUSCODEC_H

#include "AudioCodec.h"
#include <opus.h>

class OpusCodec : public AudioCodec {
public:
    OpusCodec();
    ~OpusCodec();
    std::vector<char> encode(const std::vector<float>& pcmData) override;
    std::vector<float> decode(const std::vector<char>& compressedData) override;
private:
    OpusEncoder* m_encoder = nullptr;
    OpusDecoder* m_decoder = nullptr;
    int m_frameSize;
};

#endif // OPUSCODEC_H
