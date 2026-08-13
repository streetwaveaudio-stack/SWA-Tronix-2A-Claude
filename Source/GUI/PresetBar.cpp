#include "PresetBar.h"

PresetBar::PresetBar()
{
    addAndMakeVisible (prevButton);
    addAndMakeVisible (presetCombo);
    addAndMakeVisible (nextButton);
    addAndMakeVisible (saveButton);
}

void PresetBar::resized()
{
    auto bounds = getLocalBounds();
    prevButton.setBounds (bounds.removeFromLeft (28));
    saveButton.setBounds (bounds.removeFromRight (70));
    nextButton.setBounds (bounds.removeFromRight (28));
    presetCombo.setBounds (bounds.reduced (4, 0));
}
