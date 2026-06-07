#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QAudioSource>
#include <QBuffer>
#include <QByteArray>
#include <QPushButton>
#include <QObject>
#include <memory>
#include <vector>
#include "client/AudioCodec.h"
#include "client/OpusCodec.h"

/**
 * @class AudioManager
 * @brief Class responsible for IO management of audio.
 * It handles raw audio capturing from the microphone, interacts with the audio codec
 * for compression, and manages audio playback pipelines.
 */
class AudioManager : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructor for class AudioManager.
     * Initializes the audio format and sets up the default audio codec.
     */
    AudioManager();

    /**
     * @brief Destructor for class AudioManager.
     * Cleans up allocated audio resources.
     */
    ~AudioManager() = default;

    /**
     * @brief Responsible for starting capturing pcm data from microphone and saving it to the buffer.
     */
    void startRecording();

    /**
     * @brief Responsible for stopping capturing data and compressing it using codec.
     * Then it emits signal that data is ready to be sent to server.
     */
    void stopRecording();

    /**
     * @brief Plays audio back to the user.
     * @param pcmData Input vector containing raw float PCM audio samples to be played.
     * @param playButton Optional pointer to the UI button that triggered the playback, used to track visual state.
     */
    void playAudio(const std::vector<float>& pcmData, QPushButton* playButton = nullptr);

    /**
     * @brief Getter for codec pointer.
     * @return std::shared_ptr<AudioCodec> Shared pointer to the currently utilized audio codec.
     */
    std::shared_ptr<AudioCodec> codec();

private:
    QAudioSource* m_audioSource = nullptr; /**< Qt AudioSource object which represents the microphone input device. */
    QBuffer m_buffer;                      /**< Internal buffer to store caught raw audio from the microphone. */
    QByteArray m_audioData;                /**< Byte array backing the buffer to store raw audio bytes. */
    std::shared_ptr<AudioCodec> m_codec;   /**< Pointer to codec used for compressing and decompressing data. */

signals:
    /**
     * @brief Audio is finished collecting and compressed, signal for packet sending to server.
     * @param compressedData Vector containing the compressed Opus audio payload ready for transmission.
     */
    void audioReadyToSend(const std::vector<char>& compressedData);

    /**
     * @brief Signal for changing icon on message when audio is finished playing.
     * @param playButton Pointer to the UI button that needs its icon or state reset.
     */
    void audioFinishedPlaying(QPushButton* playButton);
};

#endif // AUDIOMANAGER_H
