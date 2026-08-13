#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>

struct FactoryPreset
{
    juce::String name;
    float gainDb;
    float peakReduction; // 0..1
    int mode;            // 0 = Compress, 1 = Limit
};

/** Gestiona presets de fabrica (fijos) y presets de usuario (guardados como XML en disco). */
class PresetManager
{
public:
    explicit PresetManager (juce::AudioProcessorValueTreeState& stateToControl);

    const std::vector<FactoryPreset>& getFactoryPresets() const { return factoryPresets; }

    void loadPresetByIndex (int index);
    int getNumPresets() const { return (int) factoryPresets.size() + userPresetNames.size(); }
    juce::String getPresetName (int index) const;

    void saveCurrentStateAsUserPreset (const juce::String& name);
    void loadUserPreset (const juce::String& name);
    juce::StringArray getUserPresetNames() const { return userPresetNames; }

private:
    juce::AudioProcessorValueTreeState& apvts;
    std::vector<FactoryPreset> factoryPresets;
    juce::StringArray userPresetNames;

    juce::File getUserPresetsDirectory() const;
    void refreshUserPresetList();
};
