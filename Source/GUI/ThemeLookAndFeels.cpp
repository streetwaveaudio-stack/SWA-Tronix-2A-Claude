#include "ThemeLookAndFeels.h"
#include <cmath>

//==============================================================================
// TEMA 1 - FOTORREALISTA
//==============================================================================
PhotorealisticLNF::PhotorealisticLNF()
{
    setColour (juce::Slider::textBoxTextColourId, juce::Colours::white);
    setColour (juce::ComboBox::backgroundColourId, juce::Colour (0xff2a2a2a));
    setColour (juce::ComboBox::textColourId, juce::Colours::white);
}

void PhotorealisticLNF::drawRotarySlider (juce::Graphics& g, int x, int y, int w, int h,
                                           float pos, float startAngle, float endAngle, juce::Slider&)
{
    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) w, (float) h).reduced (4.0f);
    auto centre = bounds.getCentre();
    const float radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    const float angle = startAngle + pos * (endAngle - startAngle);

    g.setColour (juce::Colours::black.withAlpha (0.5f));
    g.fillEllipse (bounds.translated (0.0f, 2.0f));

    juce::ColourGradient metal (juce::Colour (0xffd8d8d8), centre.x, centre.y - radius,
                                 juce::Colour (0xff8a8a8a), centre.x, centre.y + radius, false);
    metal.addColour (0.5, juce::Colour (0xffb8b8b8));
    g.setGradientFill (metal);
    g.fillEllipse (bounds);

    // Textura "cepillada": lineas radiales finas y aleatorias
    juce::Path clipPath;
    clipPath.addEllipse (bounds);
    g.saveState();
    g.reduceClipRegion (clipPath);
    g.setColour (juce::Colours::white.withAlpha (0.05f));
    juce::Random rng (12345);
    for (int i = 0; i < 80; ++i)
    {
        const float a = rng.nextFloat() * juce::MathConstants<float>::twoPi;
        g.drawLine (centre.x, centre.y, centre.x + std::cos (a) * radius, centre.y + std::sin (a) * radius, 0.4f);
    }
    g.restoreState();

    g.setColour (juce::Colours::black.withAlpha (0.6f));
    g.drawEllipse (bounds, 1.5f);
    g.setColour (juce::Colours::white.withAlpha (0.25f));
    g.drawEllipse (bounds.reduced (1.5f), 1.0f);

    juce::Path pointer;
    const float pointerLength = radius * 0.75f;
    const float pointerThickness = 3.0f;
    pointer.addRoundedRectangle (-pointerThickness * 0.5f, -radius + 4.0f, pointerThickness, pointerLength, 1.5f);
    pointer.applyTransform (juce::AffineTransform::rotation (angle).translated (centre));
    g.setColour (juce::Colours::white);
    g.fillPath (pointer);
    g.setColour (juce::Colours::black.withAlpha (0.5f));
    g.strokePath (pointer, juce::PathStrokeType (0.5f));
}

void PhotorealisticLNF::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                                           bool isHighlighted, bool isDown)
{
    juce::ignoreUnused (isHighlighted, isDown);
    auto bounds = button.getLocalBounds().toFloat().reduced (2.0f);

    g.setColour (juce::Colour (0xff1a1a1a));
    g.fillRoundedRectangle (bounds, 4.0f);
    g.setColour (juce::Colours::black);
    g.drawRoundedRectangle (bounds, 4.0f, 1.0f);

    const bool on = button.getToggleState();
    auto leverArea = bounds.reduced (4.0f);
    auto leverBounds = on ? leverArea.removeFromTop (leverArea.getHeight() * 0.5f)
                           : leverArea.removeFromBottom (leverArea.getHeight() * 0.5f);

    juce::ColourGradient leverGrad (juce::Colour (0xffe0e0e0), leverBounds.getTopLeft(),
                                     juce::Colour (0xff909090), leverBounds.getBottomLeft(), false);
    g.setGradientFill (leverGrad);
    g.fillRoundedRectangle (leverBounds.reduced (1.0f), 2.0f);
    g.setColour (juce::Colours::black.withAlpha (0.6f));
    g.drawRoundedRectangle (leverBounds.reduced (1.0f), 2.0f, 1.0f);

    g.setColour (juce::Colours::white.withAlpha (0.85f));
    g.setFont (10.0f);
    g.drawText (button.getButtonText(), bounds.withY (bounds.getBottom() + 2.0f).withHeight (12.0f),
                juce::Justification::centred);
}

void PhotorealisticLNF::drawComboBox (juce::Graphics& g, int width, int height, bool,
                                       int, int, int, int, juce::ComboBox&)
{
    juce::Rectangle<float> bounds (0, 0, (float) width, (float) height);
    g.setColour (juce::Colour (0xff2a2a2a));
    g.fillRoundedRectangle (bounds, 3.0f);
    g.setColour (juce::Colour (0xffb8b8b8));
    g.drawRoundedRectangle (bounds.reduced (0.5f), 3.0f, 1.0f);

    juce::Path arrow;
    auto arrowZone = bounds.removeFromRight (20.0f).reduced (6.0f);
    arrow.addTriangle (arrowZone.getX(), arrowZone.getY(),
                        arrowZone.getRight(), arrowZone.getY(),
                        arrowZone.getCentreX(), arrowZone.getBottom());
    g.setColour (juce::Colours::white.withAlpha (0.8f));
    g.fillPath (arrow);
}

//==============================================================================
// TEMA 2 - MINIMALISTA
//==============================================================================
MinimalLNF::MinimalLNF()
{
    setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xff2b2b2b));
    setColour (juce::ComboBox::backgroundColourId, juce::Colours::white);
    setColour (juce::ComboBox::textColourId, juce::Colour (0xff2b2b2b));
}

void MinimalLNF::drawRotarySlider (juce::Graphics& g, int x, int y, int w, int h,
                                    float pos, float startAngle, float endAngle, juce::Slider&)
{
    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) w, (float) h).reduced (6.0f);
    const float radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    auto centre = bounds.getCentre();
    const float angle = startAngle + pos * (endAngle - startAngle);

    const juce::Colour track (0xffE4E4E4);
    const juce::Colour fill  (0xff2E86AB);

    juce::Path arcTrack;
    arcTrack.addCentredArc (centre.x, centre.y, radius, radius, 0.0f, startAngle, endAngle, true);
    g.setColour (track);
    g.strokePath (arcTrack, juce::PathStrokeType (5.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path arcValue;
    arcValue.addCentredArc (centre.x, centre.y, radius, radius, 0.0f, startAngle, angle, true);
    g.setColour (fill);
    g.strokePath (arcValue, juce::PathStrokeType (5.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    auto knobPos = centre + juce::Point<float> (std::sin (angle), -std::cos (angle)) * (radius * 0.7f);
    g.setColour (juce::Colour (0xff2b2b2b));
    g.fillEllipse (juce::Rectangle<float> (6.0f, 6.0f).withCentre (knobPos));
}

void MinimalLNF::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button, bool, bool)
{
    auto bounds = button.getLocalBounds().toFloat().reduced (2.0f);
    const bool on = button.getToggleState();
    g.setColour (on ? juce::Colour (0xff2E86AB) : juce::Colour (0xffE4E4E4));
    g.fillRoundedRectangle (bounds, bounds.getHeight() * 0.5f);

    auto knobArea = bounds.reduced (2.0f);
    auto knob = on ? knobArea.removeFromRight (knobArea.getHeight()) : knobArea.removeFromLeft (knobArea.getHeight());
    g.setColour (juce::Colours::white);
    g.fillEllipse (knob);

    g.setColour (juce::Colour (0xff2b2b2b));
    g.setFont (10.0f);
    g.drawText (button.getButtonText(), bounds.withY (bounds.getBottom() + 2.0f).withHeight (12.0f),
                juce::Justification::centred);
}

//==============================================================================
// TEMA 3 - NEON MORADO
//==============================================================================
NeonLNF::NeonLNF()
{
    setColour (juce::Slider::textBoxTextColourId, juce::Colours::white);
    setColour (juce::ComboBox::backgroundColourId, juce::Colour (0xff1a0f2b));
    setColour (juce::ComboBox::textColourId, juce::Colours::white);
}

void NeonLNF::drawRotarySlider (juce::Graphics& g, int x, int y, int w, int h,
                                 float pos, float startAngle, float endAngle, juce::Slider&)
{
    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) w, (float) h).reduced (6.0f);
    const float radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    auto centre = bounds.getCentre();
    const float angle = startAngle + pos * (endAngle - startAngle);

    g.setColour (juce::Colour (0x33241636));
    g.fillEllipse (bounds);
    g.setColour (juce::Colours::white.withAlpha (0.08f));
    g.drawEllipse (bounds, 1.0f);

    const juce::Colour neon (0xffb14aff);
    for (int i = 3; i >= 0; --i)
    {
        juce::Path arcValue;
        arcValue.addCentredArc (centre.x, centre.y, radius, radius, 0.0f, startAngle, angle, true);
        g.setColour (neon.withAlpha (0.18f - (float) i * 0.03f));
        g.strokePath (arcValue, juce::PathStrokeType (6.0f + (float) i * 3.0f,
                                                        juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }
    juce::Path arcValueCore;
    arcValueCore.addCentredArc (centre.x, centre.y, radius, radius, 0.0f, startAngle, angle, true);
    g.setColour (juce::Colour (0xffe0aaff));
    g.strokePath (arcValueCore, juce::PathStrokeType (3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path track;
    track.addCentredArc (centre.x, centre.y, radius, radius, 0.0f, startAngle, endAngle, true);
    g.setColour (juce::Colours::white.withAlpha (0.08f));
    g.strokePath (track, juce::PathStrokeType (3.0f));

    auto indicatorPos = centre + juce::Point<float> (std::sin (angle), -std::cos (angle)) * (radius * 0.85f);
    g.setColour (juce::Colours::white);
    g.fillEllipse (juce::Rectangle<float> (5.0f, 5.0f).withCentre (indicatorPos));
}

void NeonLNF::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button, bool, bool)
{
    auto bounds = button.getLocalBounds().toFloat().reduced (2.0f);
    const bool on = button.getToggleState();

    g.setColour (juce::Colour (0x22241636));
    g.fillRoundedRectangle (bounds, 6.0f);
    g.setColour (on ? juce::Colour (0xffb14aff) : juce::Colours::white.withAlpha (0.25f));
    g.drawRoundedRectangle (bounds, 6.0f, on ? 2.0f : 1.0f);
    if (on)
    {
        g.setColour (juce::Colour (0xffb14aff).withAlpha (0.35f));
        g.fillRoundedRectangle (bounds.reduced (2.0f), 5.0f);
    }

    g.setColour (juce::Colours::white.withAlpha (0.85f));
    g.setFont (10.0f);
    g.drawText (button.getButtonText(), bounds, juce::Justification::centred);
}

//==============================================================================
// TEMA 4 - AURORA
//==============================================================================
AuroraLNF::AuroraLNF()
{
    setColour (juce::Slider::textBoxTextColourId, juce::Colours::white);
    setColour (juce::ComboBox::backgroundColourId, juce::Colours::white.withAlpha (0.08f));
    setColour (juce::ComboBox::textColourId, juce::Colours::white);
}

void AuroraLNF::drawRotarySlider (juce::Graphics& g, int x, int y, int w, int h,
                                   float pos, float startAngle, float endAngle, juce::Slider&)
{
    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) w, (float) h).reduced (6.0f);
    const float radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    auto centre = bounds.getCentre();
    const float angle = startAngle + pos * (endAngle - startAngle);

    g.setColour (juce::Colours::white.withAlpha (0.10f));
    g.fillEllipse (bounds);
    g.setColour (juce::Colours::white.withAlpha (0.35f));
    g.drawEllipse (bounds, 1.2f);

    juce::Path arcValue;
    arcValue.addCentredArc (centre.x, centre.y, radius, radius, 0.0f, startAngle, angle, true);
    juce::ColourGradient grad (juce::Colour (0xff6ee7ff), centre.x - radius, centre.y,
                                juce::Colour (0xffb14aff), centre.x + radius, centre.y, false);
    g.setGradientFill (grad);
    g.strokePath (arcValue, juce::PathStrokeType (5.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    auto indicatorPos = centre + juce::Point<float> (std::sin (angle), -std::cos (angle)) * (radius * 0.8f);
    g.setColour (juce::Colours::white);
    g.fillEllipse (juce::Rectangle<float> (4.0f, 4.0f).withCentre (indicatorPos));
}

void AuroraLNF::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button, bool, bool)
{
    auto bounds = button.getLocalBounds().toFloat().reduced (2.0f);
    const bool on = button.getToggleState();
    g.setColour (juce::Colours::white.withAlpha (on ? 0.28f : 0.12f));
    g.fillRoundedRectangle (bounds, 6.0f);
    g.setColour (juce::Colours::white.withAlpha (0.5f));
    g.drawRoundedRectangle (bounds, 6.0f, 1.0f);

    g.setColour (juce::Colours::white.withAlpha (0.9f));
    g.setFont (10.0f);
    g.drawText (button.getButtonText(), bounds, juce::Justification::centred);
}
