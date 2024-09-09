#pragma once

#include "SmoothValueLog2.hpp"
#include <semaphore>

struct SharedProps {
    struct Audio {
        int CHANNEL_COUNT_IN = 0;
        int CHANNEL_COUNT_OUT = 2;
        int SAMPLE_RATE = 48000;
        SmoothValueLog2 freqSmoothed {0.f, SAMPLE_RATE};
        bool shouldStop = false;
    };
    Audio audio;
    std::binary_semaphore startAudio {0};
};