#include "PresetManager.h"
#include "FactoryPresets.h"
#include "../PluginProcessor.h"

PresetManager::PresetManager (juce::AudioProcessorValueTreeState& stateToControl) : apvts (stateToControl)
{
    factoryPresets = FactoryPresets::getAll();
    refreshUserPresetList();
}

juce::File PresetManager::getUserPresetsDirectory() const
{
    auto dir = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                    .getChildFile ("StreetWave Audio")
                    .getChildFile ("SWA Tronix 2A")
                    .getChildFile ("Presets");
    if (! dir.exists())
        dir.createDirectory();
    return dir;
}

void PresetManager::refreshUserPresetList()
{
    userPresetNames.clear();
    for (auto& f : getUserPresetsDirectory().findChildFiles (juce::File::findFiles, false, "*.xml"))
        userPresetNames.add (f.getFileNameWithoutExtension());
}

juce::String PresetManager::getPresetName (int index) const
{
    if (index < (int) factoryPresets.size())
        return factoryPresets[(size_t) index].name;

    const int userIndex = index - (int) factoryPresets.size();
    if (userIndex >= 0 && userIndex < userPresetNames.size())
        return userPresetNames[userIndex];

    return {};
}

void PresetManager::loadPresetByIndex (int index)
{
    if (index < (int) factoryPresets.size())
    {
        const auto& p = factoryPresets[(size_t) index];

        if (auto* gain = apvts.getParameter (SWATronix2AAudioProcessor::PARAM_GAIN))
            gain->setValueNotifyingHost (gain->convertTo0to1 (p.gainDb));

        if (auto* peak = apvts.getParameter (SWATronix2AAudioProcessor::PARAM_PEAK_REDUCTION))
            peak->setValueNotifyingHost (p.peakReduction);

        if (auto* modeParam = apvts.getParameter (SWATronix2AAudioProcessor::PARAM_MODE))
            modeParam->setValueNotifyingHost ((float) p.mode);
    }
    else
    {
        loadUserPreset (getPresetName (index));
    }
}

void PresetManager::saveCurrentStateAsUserPreset (const juce::String& name)
{
    auto state = apvts.copyState();
    if (auto xml = state.createXml())
    {
        auto file = getUserPresetsDirectory().getChildFile (name + ".xml");
        xml->writeTo (file);
        refreshUserPresetList();
    }
}

void PresetManager::loadUserPreset (const juce::String& name)
{
    auto file = getUserPresetsDirectory().getChildFile (name + ".xml");
    if (! file.existsAsFile())
        return;

    if (auto xml = juce::XmlDocument::parse (file))
    {
        auto newState = juce::ValueTree::fromXml (*xml);
        if (newState.isValid())
            apvts.replaceState (newState);
    }
}
