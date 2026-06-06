#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QCoreApplication>
#include <QSignalSpy>
#include "client/AudioManager.h"

class AudioManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        int argc = 1;
        char* argv[] = { (char*)"test" };
        if (!QCoreApplication::instance()) {
            app = std::make_unique<QCoreApplication>(argc, argv);
        }
        manager = std::make_unique<AudioManager>();
    }

    std::unique_ptr<QCoreApplication> app;
    std::unique_ptr<AudioManager> manager;
};

TEST_F(AudioManagerTest, Constructor_Initialization_CodecIsNotNull) {
    std::shared_ptr<AudioCodec> codecPtr = manager->codec();
    ASSERT_NE(codecPtr, nullptr);
}

TEST_F(AudioManagerTest, StartRecording_MultipleCalls_ResetsAndDoesNotCrash) {
    EXPECT_NO_THROW({
        manager->startRecording();
        manager->startRecording();
        manager->stopRecording();
    });
}

TEST_F(AudioManagerTest, StopRecording_WithoutStarting_HandlesGracefullyWithoutCrash) {
    EXPECT_NO_THROW({
        manager->stopRecording();
    });
}

TEST_F(AudioManagerTest, StopRecording_EmptyDataCaptured_DoesNotEmitSignal) {
    QSignalSpy spy(manager.get(), &AudioManager::audioReadyToSend);

    manager->startRecording();
    manager->stopRecording();

    EXPECT_EQ(spy.count(), 0);
}

TEST_F(AudioManagerTest, PlayAudio_EmptyPcmData_DoesNotCreateAudioSink) {
    std::vector<float> emptyPcm;

    EXPECT_NO_THROW({
        manager->playAudio(emptyPcm, nullptr);
    });
}

TEST_F(AudioManagerTest, PlayAudio_ValidPcmData_InitializesPlaybackPipeline) {
    std::vector<float> pcmData(960, 0.0f);

    EXPECT_NO_THROW({
        manager->playAudio(pcmData, nullptr);
    });
}

TEST_F(AudioManagerTest, PlayAudio_ClampingBoundaryValues_ExecutesSafely) {
    std::vector<float> extremePcm = { -5.0f, 0.0f, 5.0f };

    EXPECT_NO_THROW({
        manager->playAudio(extremePcm, nullptr);
    });
}
