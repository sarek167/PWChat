#ifndef AUDIOCODEC_H
#define AUDIOCODEC_H

#include <vector>

/**
 * @class AudioCodec
 * @brief Abstract base class (interface) representing all audio codecs in the system.
 * It defines the pure virtual contract for encoding raw PCM audio data into compressed
 * formats and decoding them back for playback.
 */
class AudioCodec {
public:
    /**
     * @brief Virtual destructor for AudioCodec class.
     * Ensures proper cleanup of derived codec resources.
     */
    virtual ~AudioCodec() = default;

    /**
     * @brief Pure virtual method which takes pcm data captured by microphone and compresses it to transferable format.
     * @param pcmData Input vector containing raw float PCM audio samples to be compressed.
     * @return std::vector<char> Output vector containing the compressed audio bytes.
     */
    virtual std::vector<char> encode(const std::vector<float>& pcmData) = 0;

    /**
     * @brief Pure virtual method which takes compressed data (received from server) and decodes it to pcm data to be able to play it to user.
     * @param compressedData Input vector containing raw compressed bytes received from the network.
     * @return std::vector<float> Output vector containing decoded float PCM samples ready for playback.
     */
    virtual std::vector<float> decode(const std::vector<char>& compressedData) = 0;
};

#endif // AUDIOCODEC_H
