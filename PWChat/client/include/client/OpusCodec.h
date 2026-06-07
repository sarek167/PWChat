#ifndef OPUSCODEC_H
#define OPUSCODEC_H

#include "AudioCodec.h"
#include <opus.h>
#include <vector>

/**
 * @class OpusCodec
 * @brief Subclass of AudioCodec using specifically Opus library.
 * It is responsible for encoding pcm data to transferable vector of chars and decode chars to pcm.
 */
class OpusCodec : public AudioCodec {
public:
    /**
     * @brief Constructor of OpusCodec class.
     */
    OpusCodec();

    /**
     * @brief Destructor of OpusCodec class.
     */
    ~OpusCodec();

    /**
     * @brief Method which takes pcm data captured by microphone and compresses it to transferable format
     * which is vector of chars.
     * @param pcmData Input vector containing raw float PCM audio samples to be compressed.
     * @return std::vector<char> Output vector containing the compressed Opus packet data.
     */
    std::vector<char> encode(const std::vector<float>& pcmData) override;

    /**
     * @brief Method which takes compressed data (received from server) and decodes it to pcm data to be able to play it to user.
     * @param compressedData Input vector containing raw compressed Opus bytes received from the network.
     * @return std::vector<float> Output vector containing decoded float PCM samples ready for audio playback.
     */
    std::vector<float> decode(const std::vector<char>& compressedData) override;

private:
    OpusEncoder* m_encoder = nullptr; /**< Pointer to the Opus encoder state structure. */
    OpusDecoder* m_decoder = nullptr; /**< Pointer to the Opus decoder state structure. */
    int m_frameSize;                  /**< Number of samples per frame processing (e.g., 960 for 20ms at 48kHz). */
};

#endif // OPUSCODEC_H
