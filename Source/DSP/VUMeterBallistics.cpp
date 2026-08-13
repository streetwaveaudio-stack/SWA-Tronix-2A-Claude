#include "VUMeterBallistics.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <cmath>

void VUMeterBallistics::prepare (double sampleRate)
{
    sr = sampleRate;
    const float timeMs = 300.0f; // tiempo de integracion de referencia, aprox IEC 60268-17
    coeff = std::exp (-1.0f / (0.001f * timeMs * (float) sr));
    reset();
}

void VUMeterBallistics::reset()
{
    smoothedLevel = 0.0f;
    currentDb = -60.0f;
}

float VUMeterBallistics::process (float inputLevelLinear)
{
    smoothedLevel = inputLevelLinear + (smoothedLevel - inputLevelLinear) * coeff;
    currentDb = juce::Decibels::gainToDecibels (smoothedLevel, -60.0f);
    return currentDb;
}
