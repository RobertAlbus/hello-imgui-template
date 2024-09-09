#pragma once

#include <algorithm>
#include <cmath>
#include <cstdio>
struct SmoothValueLog2 {
    float m_start {};
    float m_target {};
    float m_current {};
    float m_durationSeconds {};
    int m_durationSamples {};
    int m_elapsedDurationSamples {};
    int m_sampleRate {};

    SmoothValueLog2(float start, int sampleRate, float durationSeconds = 1.f) {
        if (start > -1.f && start < 0) start = -1.f;
        else if (start < 1.f && start >= 0) start = 1.f;

        m_sampleRate = sampleRate;
        set(start, durationSeconds);
        skipSmoothing();
    }

    void set(float target, float durationSeconds) {
        setTarget(target);
        setDuration(durationSeconds);
    }
    void skipSmoothing() {
        m_start = m_target;
        m_current = m_target;
        m_elapsedDurationSamples = m_durationSamples;
    }

    void setTarget(float target) {
        if (target > -1.f && target < 0) target = -1.f;
        else if (target < 1.f && target >= 0) target = 1.f;

        m_target = target;
        m_start = m_current;
        m_elapsedDurationSamples = 0;
    }

    void setDuration(float durationSeconds) {
        m_durationSeconds = std::max(durationSeconds, 0.f);
        int secondsToSamples = static_cast<int>(
            (durationSeconds * static_cast<float>(m_sampleRate)) - 1
        );
        m_durationSamples = std::max(1,secondsToSamples);
        setTarget(m_target);
    }

    float next() {
        if (m_elapsedDurationSamples >= m_durationSamples) {
            m_start = m_target;
            m_current = m_target;
            return m_current;
        }
        
        float smoothingPosition =
            static_cast<float>(m_elapsedDurationSamples++)
            / static_cast<float>(m_durationSamples);

        if (smoothingPosition == 0.f) {
            m_current = m_start;
            return m_current;
        }

        m_current = m_start * pow(2, smoothingPosition * log2(m_target / m_start));
        return m_current;
    }
};