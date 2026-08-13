#pragma once
#include <juce_audio_basics/juce_audio_basics.h>

/** Saturacion sutil tipo valvula: waveshaper asimetrico que enfatiza armonicos pares. */
class TubeStage
{
public:
    void prepare (double sampleRate);
    void reset();
    void process (juce::AudioBuffer<float>& buffer, float driveAmount);

private:
    double sr = 44100.0;
};
