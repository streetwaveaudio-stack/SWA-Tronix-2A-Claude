#include "OutputTransformer.h"
#include <cmath>

void OutputTransformer::prepare (double sampleRate)
{
    sr = sampleRate;
    for (int ch = 0; ch < 2; ++ch)
    {
        lowShelf[ch].coefficients  = juce::dsp::IIR::Coefficients<float>::makeLowShelf (sr, 120.0f, 0.7f, 1.15f);
        hfRolloff[ch].coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf (sr, 9000.0f, 0.7f, 0.9f);
    }
    reset();
}

void OutputTransformer::reset()
{
    for (int ch = 0; ch < 2; ++ch)
    {
        lowShelf[ch].reset();
        hfRolloff[ch].reset();
    }
}

void OutputTransformer::process (juce::AudioBuffer<float>& buffer, float amount)
{
    for (int ch = 0; ch < buffer.getNumChannels() && ch < 2; ++ch)
    {
        auto* data = buffer.getWritePointer (ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float x = data[i];
            x = lowShelf[ch].processSample (x);
            x = x + amount * 0.06f * (std::tanh (x * 3.0f) - x); // saturacion suave en graves
            x = hfRolloff[ch].processSample (x);
            data[i] = x;
        }
    }
}
