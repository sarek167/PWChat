#include "client/AudioManager.h"
#include <QAudioFormat>
#include <QAudioSink>
#include <QMediaDevices>
#include <QAudio>
#include "client/OpusCodec.h"
#include <iostream>

AudioManager::AudioManager() {
    m_codec = std::make_unique<OpusCodec>();
}

std::shared_ptr<AudioCodec> AudioManager::codec() {
    return m_codec;
}

void AudioManager::startRecording() {
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    m_audioData.clear();

    m_buffer.setBuffer(&m_audioData);
    if (m_audioSource) {
        m_audioSource->deleteLater();
    }

    m_audioSource = new QAudioSource(format, this);
    if (m_buffer.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        m_audioSource->start(&m_buffer);
        std::cout << "Recording started in audio manager" << std::endl;
    } else {
        std::cerr << "Error while opening buffor" << std::endl;
    }

}

void AudioManager::stopRecording() {
    m_audioSource->stop();
    m_buffer.close();
    if (m_audioData.isEmpty()) {
        std::cerr << "No audio data captured!" << std::endl;
        return;
    }

    const QByteArray &rawData = m_audioData;

    if (rawData.isEmpty()) return;

    int sampleCount = rawData.size() / sizeof(int16_t);
    std::vector<float> pcmData;
    pcmData.reserve(sampleCount);

    const int16_t* samples = reinterpret_cast<const int16_t*>(rawData.constData());
    for (int i = 0; i < sampleCount; ++i) {
        pcmData.push_back(samples[i] / 32768.0f);
    }

    std::vector<char> compressed = m_codec->encode(pcmData);
    std::cout << "Compressed size:" << compressed.size();

    if (compressed.empty()) {
        std::cerr << "Codec returned empty vector!";
        return;
    }
    emit audioReadyToSend(compressed);
}

void AudioManager::playAudio(const std::vector<float>& pcmData) {
    if (pcmData.empty()) return;

    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QByteArray audioBuffer;
    audioBuffer.reserve(pcmData.size() * sizeof(int16_t));

    for (float sample : pcmData) {
        float clamped = std::max(-1.0f, std::min(1.0f, sample));
        int16_t s = static_cast<int16_t>(clamped * 32767.0f);
        audioBuffer.append(reinterpret_cast<const char*>(&s), sizeof(int16_t));
    }

    auto* audioSink = new QAudioSink(format, this);

    QBuffer* buffer = new QBuffer(this);
    buffer->setData(audioBuffer);
    buffer->open(QIODevice::ReadOnly);

    audioSink->start(buffer);

    connect(audioSink, &QAudioSink::stateChanged, [audioSink, buffer](QAudio::State state) {
        if (state == QAudio::IdleState || state == QAudio::StoppedState) {
            audioSink->stop();
            audioSink->deleteLater();
            buffer->deleteLater();
        }
    });
}
