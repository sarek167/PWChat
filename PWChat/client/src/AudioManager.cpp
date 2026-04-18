#include "client/AudioManager.h"
#include <QAudioFormat>
#include "client/OpusCodec.h"

AudioManager::AudioManager() {
    m_codec = std::make_unique<OpusCodec>();
}

void AudioManager::startRecording() {
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    m_audioSource = new QAudioSource(format, this);
    m_buffer.open(QIODevice::ReadWrite | QIODevice::Truncate);
    m_audioSource->start(&m_buffer);
}

void AudioManager::stopRecording() {
    m_audioSource->stop();
    m_buffer.close();

    QByteArray rawData = m_audioData.data();

    if (rawData.isEmpty()) return;

    int sampleCount = rawData.size() / sizeof(int16_t);
    std::vector<float> pcmData;
    pcmData.reserve(sampleCount);

    const int16_t* samples = reinterpret_cast<const int16_t*>(rawData.constData());
    for (int i = 0; i < sampleCount; ++i) {
        pcmData.push_back(samples[i] / 32768.0f);
    }

    std::vector<char> compressed = m_codec->encode(pcmData);

    // emit audioReadyToSend(compressed);
}
