#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/Theme.h"
#include "GUI/ThemeLookAndFeels.h"
#include "GUI/VUMeterComponent.h"
#include "GUI/InfoPanel.h"
#include "GUI/PresetBar.h"
#include "GUI/AuroraBackground.h"

class SWATronix2AAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    explicit SWATronix2AAudioProcessorEditor (SWATronix2AAudioProcessor&);
    ~SWATronix2AAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void applyTheme (ThemeId theme);
    void refreshPresetCombo();

    SWATronix2AAudioProcessor& processorRef;

    PhotorealisticLNF photorealisticLNF;
    MinimalLNF minimalLNF;
    NeonLNF neonLNF;
    AuroraLNF auroraLNF;

    ThemeId currentTheme = ThemeId::Photorealistic;
    std::unique_ptr<AuroraBackground> auroraBackground;

    juce::Slider gainKnob, peakReductionKnob;
    juce::Label gainLabel, peakReductionLabel;
    juce::ToggleButton modeButton { "Limit" };
    juce::ToggleButton meterButton { "GR" };
    juce::ToggleButton powerButton { "Power" };

    juce::ComboBox themeCombo;
    juce::TextButton infoButton { "i" };
    std::unique_ptr<InfoPanel> infoPanel;

    PresetBar presetBar;
    VUMeterComponent vuMeter;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<SliderAttachment> gainAttachment, peakReductionAttachment;
    std::unique_ptr<ButtonAttachment> powerAttachment;
    std::unique_ptr<juce::ParameterAttachment> modeAttachment, meterAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SWATronix2AAudioProcessorEditor)
};
