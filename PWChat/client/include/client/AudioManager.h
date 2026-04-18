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
private:
    QAudioSource* m_audioSource = nullptr;
    QBuffer m_buffer;
    QByteArray m_audioData;
    std::unique_ptr<AudioCodec> m_codec;
};

#endif // AUDIOMANAGER_H
