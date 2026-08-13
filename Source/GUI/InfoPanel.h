#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

/** Panel modal de informacion, accesible desde cualquier tema. */
class InfoPanel : public juce::Component
{
public:
    InfoPanel();
    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent&) override;

    std::function<void()> onClose;

private:
    juce::TextButton closeButton { "Cerrar" };
    juce::Rectangle<int> cardBounds;
};
