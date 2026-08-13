#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

/** Barra de presets: combo desplegable + flechas anterior/siguiente + guardar. */
class PresetBar : public juce::Component
{
public:
    PresetBar();
    void resized() override;

    juce::ComboBox presetCombo;
    juce::TextButton prevButton { "<" };
    juce::TextButton nextButton { ">" };
    juce::TextButton saveButton { "Guardar" };
};
