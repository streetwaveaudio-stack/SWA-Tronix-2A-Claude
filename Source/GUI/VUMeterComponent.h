#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

/** Medidor con fisica de aguja amortiguada (tema fotorrealista) o variantes planas/LED/aurora. */
class VUMeterComponent : public juce::Component, private juce::Timer
{
public:
    enum class Style { AnalogNeedle, FlatBar, NeonLedBar, AuroraSubtle };

    VUMeterComponent();
    ~VUMeterComponent() override;

    void setStyle (Style newStyle) { style = newStyle; repaint(); }
    void setLevelDb (float db);
    void paint (juce::Graphics&) override;

private:
    void timerCallback() override;

    Style style = Style::AnalogNeedle;
    float targetDb = -20.0f;
    float needleVelocity = 0.0f;
    float displayLevel01 = 0.0f;
};
