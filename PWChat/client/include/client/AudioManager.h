#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QAudioSource>
#include <QBuffer>
#include <QByteArray>
#include <client/AudioCodec.h>
#include <client/OpusCodec.h>

class AudioManager : public QObject {
    Q_OBJECT
public:
    AudioManager();
    void startRecording();
    void stopRecording();
    void playAudio(const std::vector<float>& pcmData);
    std::shared_ptr<AudioCodec> codec();
private:
    QAudioSource* m_audioSource = nullptr;
    QBuffer m_buffer;
    QByteArray m_audioData;
    std::shared_ptr<AudioCodec> m_codec;

signals:
    void audioReadyToSend(const std::vector<char>& compressedData);
};

#endif // AUDIOMANAGER_H
