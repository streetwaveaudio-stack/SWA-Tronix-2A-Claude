#include "TubeStage.h"
#include <cmath>

void TubeStage::prepare (double sampleRate) { sr = sampleRate; reset(); }
void TubeStage::reset() {}

void TubeStage::process (juce::AudioBuffer<float>& buffer, float driveAmount)
{
    if (driveAmount <= 0.0001f)
        return;

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getWritePointer (ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            const float x = data[i];
            const float driven = x * (1.0f + driveAmount * 2.0f);

            // Asimetria suave: mas saturacion en el semiciclo positivo -> armonicos pares
            const float shaped = (driven >= 0.0f)
                ? std::tanh (driven * 0.9f)
                : std::tanh (driven * 1.05f);

            data[i] = x + driveAmount * (shaped - x);
        }
    }
}
