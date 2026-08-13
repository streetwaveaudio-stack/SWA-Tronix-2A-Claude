#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

/** Fondo animado estilo "aurora/blob" para el Tema 4, con color que reacciona a la reduccion de ganancia. */
class AuroraBackground : public juce::Component, private juce::Timer
{
public:
    AuroraBackground();
    ~AuroraBackground() override;

    void paint (juce::Graphics&) override;
    void setGainReductionAmount (float amount01);

private:
    void timerCallback() override;

    float phase = 0.0f;
    float grAmount = 0.0f;
};
