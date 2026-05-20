#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QAudioSource>
#include <QBuffer>
#include <QByteArray>
#include <QPushButton>
#include <client/AudioCodec.h>
#include <client/OpusCodec.h>

class AudioManager : public QObject {
    Q_OBJECT
public:
    AudioManager();
    void startRecording();
    void stopRecording();
    void playAudio(const std::vector<float>& pcmData, QPushButton* playButton = nullptr);
    std::shared_ptr<AudioCodec> codec();
private:
    QAudioSource* m_audioSource = nullptr;
    QBuffer m_buffer;
    QByteArray m_audioData;
    std::shared_ptr<AudioCodec> m_codec;

signals:
    void audioReadyToSend(const std::vector<char>& compressedData);
    void audioFinishedPlaying(QPushButton* playButton);
};

#endif // AUDIOMANAGER_H
