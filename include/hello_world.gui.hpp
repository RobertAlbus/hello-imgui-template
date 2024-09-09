#pragma once

#include "hello_imgui/hello_imgui.h"
#include "SharedProps.hpp"

void GUI(SharedProps& sharedProps) {
    float frequency = 100.f;
    float frequencyGuiSpeed = 10.f;
    float smoothingSeconds = 1.f;

    sharedProps.audio.freqSmoothed.set(frequency, smoothingSeconds);
    sharedProps.audio.freqSmoothed.skipSmoothing();

    sharedProps.startAudio.release();

    auto guiFunction = [&]() {

        ImGui::PushItemWidth(200);

        ImGui::SliderFloat("speed", &frequencyGuiSpeed, 0.f, 200.f);
        if (ImGui::DragFloat("Frequency", &frequency, frequencyGuiSpeed, 0.0f, 24000.0f, "%.2f", ImGuiSliderFlags_Logarithmic)) {
            sharedProps.audio.freqSmoothed.setTarget(frequency);
        }
        if (ImGui::SliderFloat("Smoothing", &smoothingSeconds, 0.0f, 10.f, "%.2f")) {
            sharedProps.audio.freqSmoothed.setDuration(smoothingSeconds);
        }

        ImGui::PopItemWidth();

        if (ImGui::Button("Bye!")) {
            HelloImGui::GetRunnerParams()->appShallExit = true;
        }
        ImGui::Text("Framerate: %.2f", ImGui::GetIO().Framerate);
        
    };

    HelloImGui::RunnerParams params {};
    params.callbacks.EnqueueBeforeExit([&](){
        sharedProps.audio.shouldStop = true;
    });
    params.callbacks.ShowGui = guiFunction;
    params.appWindowParams.windowTitle = "Hello, Audio";
    params.fpsIdling.enableIdling = true;
    // params.imGuiWindowParams.
    HelloImGui::Run(params);
    

}