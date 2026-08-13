#include "InfoPanel.h"

InfoPanel::InfoPanel()
{
    addAndMakeVisible (closeButton);
    closeButton.onClick = [this] { if (onClose) onClose(); };
}

void InfoPanel::resized()
{
    cardBounds = getLocalBounds().withSizeKeepingCentre (320, 220);
    auto area = cardBounds;
    closeButton.setBounds (area.removeFromBottom (36).reduced (80, 4));
}

void InfoPanel::mouseDown (const juce::MouseEvent& e)
{
    if (! cardBounds.contains (e.getPosition()) && onClose)
        onClose();
}

void InfoPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black.withAlpha (0.55f));

    auto card = cardBounds.toFloat();
    g.setColour (juce::Colour (0xff222222));
    g.fillRoundedRectangle (card, 8.0f);
    g.setColour (juce::Colours::white.withAlpha (0.15f));
    g.drawRoundedRectangle (card, 8.0f, 1.0f);

    auto textArea = card.reduced (20.0f).withTrimmedBottom (40.0f);

    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (18.0f, juce::Font::bold));
    g.drawText ("StreetWave Audio", textArea.removeFromTop (26.0f), juce::Justification::centredLeft);

    g.setFont (16.0f);
    g.drawText ("SWA Tronix 2A", textArea.removeFromTop (24.0f), juce::Justification::centredLeft);

    g.setFont (13.0f);
    g.setColour (juce::Colours::white.withAlpha (0.75f));
    g.drawText ("Version 1.0.0", textArea.removeFromTop (20.0f), juce::Justification::centredLeft);

    g.drawFittedText (
        "Emulacion de amplificador de nivelacion optico inspirada en el "
        "clasico de la era vintage. Desarrollado con JUCE.",
        textArea.removeFromTop (60.0f).toNearestInt(), juce::Justification::topLeft, 4);

    g.setColour (juce::Colours::white.withAlpha (0.5f));
    g.setFont (11.0f);
    g.drawText ("streetwaveaudio.example.com", textArea, juce::Justification::centredLeft);
}
