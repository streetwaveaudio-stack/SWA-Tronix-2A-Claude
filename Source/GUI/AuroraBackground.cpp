#include "AuroraBackground.h"
#include <cmath>

AuroraBackground::AuroraBackground() { startTimerHz (30); }
AuroraBackground::~AuroraBackground() { stopTimer(); }

void AuroraBackground::setGainReductionAmount (float amount01) { grAmount = juce::jlimit (0.0f, 1.0f, amount01); }

void AuroraBackground::timerCallback()
{
    phase += 0.01f;
    repaint();
}

void AuroraBackground::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.setColour (juce::Colour (0xff0c0a1a));
    g.fillRect (bounds);

    struct Blob { float speedX, speedY, radiusScale; juce::Colour colour; };
    const Blob blobs[] = {
        { 0.7f,  0.5f,  0.55f, juce::Colour (0xff7c3aed) },
        { -0.5f, 0.8f,  0.45f, juce::Colour (0xffec4899) },
        { 0.4f, -0.6f,  0.5f,  juce::Colour (0xff22d3ee) },
        { -0.6f,-0.4f,  0.4f,  juce::Colour (0xffb14aff) }
    };

    const float w = bounds.getWidth();
    const float h = bounds.getHeight();

    int i = 0;
    for (auto& b : blobs)
    {
        const float cx = w * 0.5f + std::sin (phase * b.speedX + (float) i) * w * 0.35f;
        const float cy = h * 0.5f + std::cos (phase * b.speedY + (float) i * 1.7f) * h * 0.35f;
        const float radius = juce::jmin (w, h) * b.radiusScale * (0.85f + grAmount * 0.3f);

        juce::ColourGradient grad (b.colour.withAlpha (0.55f + grAmount * 0.2f), cx, cy,
                                    b.colour.withAlpha (0.0f), cx + radius, cy, true);
        g.setGradientFill (grad);
        g.fillEllipse (juce::Rectangle<float> (radius * 2.0f, radius * 2.0f).withCentre ({ cx, cy }));
        ++i;
    }
}
