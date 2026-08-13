#include "VUMeterComponent.h"
#include <cmath>

VUMeterComponent::VUMeterComponent() { startTimerHz (60); }
VUMeterComponent::~VUMeterComponent() { stopTimer(); }

void VUMeterComponent::setLevelDb (float db) { targetDb = db; }

void VUMeterComponent::timerCallback()
{
    const float target01 = juce::jlimit (0.0f, 1.0f, juce::jmap (targetDb, -20.0f, 3.0f, 0.0f, 1.0f));

    // Fisica de aguja amortiguada (sistema masa-resorte)
    const float stiffness = 55.0f;
    const float damping = 9.5f;
    const float dt = 1.0f / 60.0f;
    const float force = (target01 - displayLevel01) * stiffness - needleVelocity * damping;
    needleVelocity += force * dt;
    displayLevel01 = juce::jlimit (0.0f, 1.0f, displayLevel01 + needleVelocity * dt);

    repaint();
}

void VUMeterComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    switch (style)
    {
        case Style::AnalogNeedle:
        {
            g.setColour (juce::Colour (0xfff2ecd8));
            g.fillRoundedRectangle (bounds, 4.0f);
            g.setColour (juce::Colours::black);
            g.drawRoundedRectangle (bounds.reduced (1.0f), 4.0f, 1.5f);

            auto centre = juce::Point<float> (bounds.getCentreX(), bounds.getBottom() - 6.0f);
            const float radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.85f;
            const float minAngle = juce::degreesToRadians (-50.0f);
            const float maxAngle = juce::degreesToRadians (50.0f);

            g.setColour (juce::Colours::black.withAlpha (0.7f));
            for (int i = 0; i <= 10; ++i)
            {
                const float t = (float) i / 10.0f;
                const float a = minAngle + t * (maxAngle - minAngle);
                const auto p1 = centre + juce::Point<float> (std::sin (a), -std::cos (a)) * (radius * 0.9f);
                const auto p2 = centre + juce::Point<float> (std::sin (a), -std::cos (a)) * radius;
                g.drawLine ({ p1, p2 }, t > 0.75f ? 2.0f : 1.0f);
            }

            const float needleAngleRad = minAngle + displayLevel01 * (maxAngle - minAngle);
            juce::Path needle;
            needle.startNewSubPath (centre);
            needle.lineTo (centre + juce::Point<float> (std::sin (needleAngleRad), -std::cos (needleAngleRad)) * (radius * 0.92f));
            g.setColour (juce::Colours::black);
            g.strokePath (needle, juce::PathStrokeType (1.6f));
            g.fillEllipse (juce::Rectangle<float> (6.0f, 6.0f).withCentre (centre));
            break;
        }
        case Style::FlatBar:
        {
            g.setColour (juce::Colour (0xffE4E4E4));
            g.fillRoundedRectangle (bounds, 3.0f);
            auto fillArea = bounds.reduced (2.0f);
            fillArea = fillArea.removeFromBottom (fillArea.getHeight() * displayLevel01);
            g.setColour (juce::Colour (0xff2E86AB));
            g.fillRoundedRectangle (fillArea, 2.0f);
            break;
        }
        case Style::NeonLedBar:
        {
            const int numLeds = 14;
            const float ledH = bounds.getHeight() / (float) numLeds;
            const int litLeds = juce::roundToInt (displayLevel01 * (float) numLeds);
            for (int i = 0; i < numLeds; ++i)
            {
                const bool lit = i >= (numLeds - litLeds);
                auto ledBounds = bounds.removeFromBottom (ledH).reduced (2.0f, 1.0f);
                const float t = (float) i / (float) numLeds;
                auto colour = juce::Colour::fromHSV (0.78f - t * 0.15f, 0.85f, 1.0f, 1.0f);
                g.setColour (lit ? colour : colour.withAlpha (0.12f));
                g.fillRoundedRectangle (ledBounds, 2.0f);
            }
            break;
        }
        case Style::AuroraSubtle:
        {
            g.setColour (juce::Colours::white.withAlpha (0.15f));
            g.fillRoundedRectangle (bounds, bounds.getHeight() * 0.5f);
            auto fillArea = bounds.reduced (2.0f);
            fillArea = fillArea.removeFromLeft (fillArea.getWidth() * displayLevel01);
            juce::ColourGradient grad (juce::Colour (0xff6ee7ff), fillArea.getX(), 0.0f,
                                        juce::Colour (0xffb14aff), fillArea.getRight(), 0.0f, false);
            g.setGradientFill (grad);
            g.fillRoundedRectangle (fillArea, fillArea.getHeight() * 0.5f);
            break;
        }
    }
}
