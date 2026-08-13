#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

/** Coloracion del transformador de salida: ligera saturacion en graves y suavizado de agudos. */
class OutputTransformer
{
public:
    void prepare (double sampleRate);
    void reset();
    void process (juce::AudioBuffer<float>& buffer, float amount);

private:
    double sr = 44100.0;
    juce::dsp::IIR::Filter<float> lowShelf[2];
    juce::dsp::IIR::Filter<float> hfRolloff[2];
};
