#ifndef AUDIOCODEC_H
#define AUDIOCODEC_H

#include <vector>

class AudioCodec {
public:
    ~AudioCodec() = default;
    virtual std::vector<char> encode(const std::vector<float>& pcmData) = 0;
    virtual std::vector<float> decode(const std::vector<char>& compressedData) = 0;

};

#endif // AUDIOCODEC_H
