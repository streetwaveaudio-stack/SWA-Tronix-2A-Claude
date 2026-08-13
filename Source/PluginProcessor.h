#pragma once
#include <JuceHeader.h>
#include "DSP/LA2ACompressor.h"
#include "Presets/PresetManager.h"

class SWATronix2AAudioProcessor : public juce::AudioProcessor
{
public:
    SWATronix2AAudioProcessor();
    ~SWATronix2AAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 5.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;
    LA2ACompressor compressorEngine;
    PresetManager presetManager;

    // Valores de medicion expuestos al editor (thread-safe)
    std::atomic<float> currentGainReductionDb { 0.0f };
    std::atomic<float> currentOutputLevelDb { -60.0f };

    // Tema visual seleccionado, persistido junto al resto del estado
    std::atomic<int> selectedThemeIndex { 0 };

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    static constexpr const char* PARAM_GAIN = "gain";
    static constexpr const char* PARAM_PEAK_REDUCTION = "peakReduction";
    static constexpr const char* PARAM_MODE = "mode";
    static constexpr const char* PARAM_METER = "meter";
    static constexpr const char* PARAM_POWER = "power";

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SWATronix2AAudioProcessor)
};
