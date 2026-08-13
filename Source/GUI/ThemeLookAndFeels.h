#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

/** Tema 1: fotorrealista. Replica el panel del LA-2A con aluminio cepillado procedural. */
class PhotorealisticLNF : public juce::LookAndFeel_V4
{
public:
    PhotorealisticLNF();
    void drawRotarySlider (juce::Graphics&, int x, int y, int w, int h,
                            float sliderPosProportional, float rotaryStartAngle,
                            float rotaryEndAngle, juce::Slider&) override;
    void drawToggleButton (juce::Graphics&, juce::ToggleButton&,
                            bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    void drawComboBox (juce::Graphics&, int width, int height, bool isButtonDown,
                        int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox&) override;
};

/** Tema 2: moderno minimalista. Arcos de progreso planos, paleta reducida. */
class MinimalLNF : public juce::LookAndFeel_V4
{
public:
    MinimalLNF();
    void drawRotarySlider (juce::Graphics&, int x, int y, int w, int h,
                            float sliderPosProportional, float rotaryStartAngle,
                            float rotaryEndAngle, juce::Slider&) override;
    void drawToggleButton (juce::Graphics&, juce::ToggleButton&,
                            bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};

/** Tema 3: neon morado. Anillos luminosos, glassmorphism sutil. */
class NeonLNF : public juce::LookAndFeel_V4
{
public:
    NeonLNF();
    void drawRotarySlider (juce::Graphics&, int x, int y, int w, int h,
                            float sliderPosProportional, float rotaryStartAngle,
                            float rotaryEndAngle, juce::Slider&) override;
    void drawToggleButton (juce::Graphics&, juce::ToggleButton&,
                            bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};

/** Tema 4: aurora animada. Controles semitransparentes flotando sobre el fondo. */
class AuroraLNF : public juce::LookAndFeel_V4
{
public:
    AuroraLNF();
    void drawRotarySlider (juce::Graphics&, int x, int y, int w, int h,
                            float sliderPosProportional, float rotaryStartAngle,
                            float rotaryEndAngle, juce::Slider&) override;
    void drawToggleButton (juce::Graphics&, juce::ToggleButton&,
                            bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};
