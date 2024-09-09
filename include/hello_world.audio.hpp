#pragma once

#include <cmath>
#include <numbers>

#include "Clover/AudioCallback.hpp"
#include "Clover/Interface.hpp"
#include "SharedProps.hpp"



float calculatePhaseIncrement(float freq, int sampleRate) {
    if (freq == 0.f) return 0.f;
    return 2.f * std::numbers::pi * freq / static_cast<float>(sampleRate);
}


void AUDIO(SharedProps& sharedProps) {
    sharedProps.startAudio.acquire();
    
    float increment = 0.f;
    float frequency = sharedProps.audio.freqSmoothed.m_current;
    float theta = 0.f;

    increment = calculatePhaseIncrement(sharedProps.audio.freqSmoothed.m_current, sharedProps.audio.SAMPLE_RATE);
    frequency = sharedProps.audio.freqSmoothed.m_current;


    auto audioCallback = [&sharedProps, &increment, &frequency, &theta](Clover::AudioCallbackArguments data) {

        // Update composition data
        float smoothedFrequency = sharedProps.audio.freqSmoothed.next();
        // printf("AUDIO: callback - smoothed frequency %f\n", smoothedFrequency);
        if (frequency != smoothedFrequency) {
            frequency = smoothedFrequency;
            increment = calculatePhaseIncrement(frequency, sharedProps.audio.SAMPLE_RATE);
        }

        // Generate signal
        float signal = std::sinf(theta);
        theta += increment;
        if (theta > 40 * std::numbers::pi_v<float>) {
            theta -= 40 * std::numbers::pi_v<float>;
        }

        // Assign signal to output buffer
        float &L = *(data.output);
        float &R = *(data.output + 1);
        L = signal;
        R = signal;

        // Return
        if (sharedProps.audio.shouldStop) {
            return Clover::AudioCallbackStatus::END;
        }
        return Clover::AudioCallbackStatus::CONTINUE;

    };


    Clover::Interface interface;
    Clover::SystemAudioIoConfig systemAudioConfig = Clover::GetSystemAudioIoConfig();
    // systemAudioConfig.printDevices();

    try {
        interface.openDevice({
        -1,
        sharedProps.audio.CHANNEL_COUNT_IN,
        systemAudioConfig.defaultOutputDeviceIndex,
        sharedProps.audio.CHANNEL_COUNT_OUT,
        sharedProps.audio.SAMPLE_RATE,
        0
    });
    } catch(Clover::Exception e) {
        printf("%s\n", e.what());
        return;
    }

    interface.setAudioCallback(audioCallback);

    try {
        interface.start();
    } catch (Clover::Exception e) {
        printf("%s\n", e.what());
        return;
    }

    interface.waitForStreamComplete();
    interface.stop();
}