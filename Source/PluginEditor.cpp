#include "PluginEditor.h"

SWATronix2AAudioProcessorEditor::SWATronix2AAudioProcessorEditor (SWATronix2AAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    setResizable (true, true);
    setResizeLimits (620, 320, 1400, 720);
    setSize (900, 420);

    for (auto* knob : { &gainKnob, &peakReductionKnob })
    {
        knob->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        knob->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 16);
        addAndMakeVisible (knob);
    }

    gainLabel.setText ("Gain", juce::dontSendNotification);
    peakReductionLabel.setText ("Peak Reduction", juce::dontSendNotification);
    for (auto* label : { &gainLabel, &peakReductionLabel })
    {
        label->setJustificationType (juce::Justification::centred);
        addAndMakeVisible (label);
    }

    addAndMakeVisible (modeButton);
    addAndMakeVisible (meterButton);
    addAndMakeVisible (powerButton);

    themeCombo.addItem ("Fotorrealista", 1);
    themeCombo.addItem ("Minimalista", 2);
    themeCombo.addItem ("Neon Morado", 3);
    themeCombo.addItem ("Aurora", 4);
    themeCombo.setSelectedId (processorRef.selectedThemeIndex.load() + 1, juce::dontSendNotification);
    themeCombo.onChange = [this]
    {
        const auto theme = (ThemeId) (themeCombo.getSelectedId() - 1);
        processorRef.selectedThemeIndex.store ((int) theme);
        applyTheme (theme);
    };
    addAndMakeVisible (themeCombo);

    addAndMakeVisible (infoButton);
    infoButton.onClick = [this]
    {
        infoPanel = std::make_unique<InfoPanel>();
        infoPanel->onClose = [this] { infoPanel.reset(); };
        addAndMakeVisible (*infoPanel);
        infoPanel->setBounds (getLocalBounds());
    };

    addAndMakeVisible (presetBar);
    refreshPresetCombo();
    presetBar.presetCombo.onChange = [this]
    {
        processorRef.presetManager.loadPresetByIndex (presetBar.presetCombo.getSelectedItemIndex());
    };
    presetBar.prevButton.onClick = [this]
    {
        const auto idx = juce::jmax (0, presetBar.presetCombo.getSelectedItemIndex() - 1);
        presetBar.presetCombo.setSelectedItemIndex (idx);
    };
    presetBar.nextButton.onClick = [this]
    {
        const auto idx = juce::jmin (presetBar.presetCombo.getNumItems() - 1, presetBar.presetCombo.getSelectedItemIndex() + 1);
        presetBar.presetCombo.setSelectedItemIndex (idx);
    };
    presetBar.saveButton.onClick = [this]
    {
        const auto name = "Preset de usuario " + juce::String (juce::Time::getCurrentTime().toMilliseconds());
        processorRef.presetManager.saveCurrentStateAsUserPreset (name);
        refreshPresetCombo();
    };

    addAndMakeVisible (vuMeter);

    auto& apvts = processorRef.apvts;
    gainAttachment = std::make_unique<SliderAttachment> (apvts, SWATronix2AAudioProcessor::PARAM_GAIN, gainKnob);
    peakReductionAttachment = std::make_unique<SliderAttachment> (apvts, SWATronix2AAudioProcessor::PARAM_PEAK_REDUCTION, peakReductionKnob);
    powerAttachment = std::make_unique<ButtonAttachment> (apvts, SWATronix2AAudioProcessor::PARAM_POWER, powerButton);

    if (auto* modeParam = apvts.getParameter (SWATronix2AAudioProcessor::PARAM_MODE))
    {
        modeAttachment = std::make_unique<juce::ParameterAttachment> (*modeParam, [this] (float v)
        {
            modeButton.setToggleState (v > 0.5f, juce::dontSendNotification);
        });
        modeButton.onClick = [this]
        {
            modeAttachment->setValueAsCompleteGesture (modeButton.getToggleState() ? 1.0f : 0.0f);
        };
        modeAttachment->sendInitialUpdate();
    }

    if (auto* meterParam = apvts.getParameter (SWATronix2AAudioProcessor::PARAM_METER))
    {
        meterAttachment = std::make_unique<juce::ParameterAttachment> (*meterParam, [this] (float v)
        {
            meterButton.setToggleState (v > 0.5f, juce::dontSendNotification);
        });
        meterButton.onClick = [this]
        {
            meterAttachment->setValueAsCompleteGesture (meterButton.getToggleState() ? 1.0f : 0.0f);
        };
        meterAttachment->sendInitialUpdate();
    }

    applyTheme ((ThemeId) processorRef.selectedThemeIndex.load());
    startTimerHz (30);
}

SWATronix2AAudioProcessorEditor::~SWATronix2AAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void SWATronix2AAudioProcessorEditor::refreshPresetCombo()
{
    presetBar.presetCombo.clear (juce::dontSendNotification);
    for (int i = 0; i < processorRef.presetManager.getNumPresets(); ++i)
        presetBar.presetCombo.addItem (processorRef.presetManager.getPresetName (i), i + 1);
    presetBar.presetCombo.setSelectedItemIndex (0, juce::dontSendNotification);
}

void SWATronix2AAudioProcessorEditor::applyTheme (ThemeId theme)
{
    currentTheme = theme;

    juce::LookAndFeel* lnf = &photorealisticLNF;
    auto meterStyle = VUMeterComponent::Style::AnalogNeedle;

    switch (theme)
    {
        case ThemeId::Photorealistic: lnf = &photorealisticLNF; meterStyle = VUMeterComponent::Style::AnalogNeedle;  break;
        case ThemeId::Minimal:        lnf = &minimalLNF;        meterStyle = VUMeterComponent::Style::FlatBar;      break;
        case ThemeId::Neon:           lnf = &neonLNF;            meterStyle = VUMeterComponent::Style::NeonLedBar;  break;
        case ThemeId::Aurora:         lnf = &auroraLNF;          meterStyle = VUMeterComponent::Style::AuroraSubtle; break;
    }

    setLookAndFeel (lnf);
    vuMeter.setStyle (meterStyle);

    if (theme == ThemeId::Aurora)
    {
        if (auroraBackground == nullptr)
        {
            auroraBackground = std::make_unique<AuroraBackground>();
            addAndMakeVisible (*auroraBackground);
            auroraBackground->toBack();
        }
        auroraBackground->setVisible (true);
        auroraBackground->toBack();
    }
    else if (auroraBackground != nullptr)
    {
        auroraBackground->setVisible (false);
    }

    resized();
    repaint();
}

void SWATronix2AAudioProcessorEditor::timerCallback()
{
    const bool showGr = *processorRef.apvts.getRawParameterValue (SWATronix2AAudioProcessor::PARAM_METER) > 0.5f;
    const float db = showGr ? -processorRef.currentGainReductionDb.load() : processorRef.currentOutputLevelDb.load();
    vuMeter.setLevelDb (db);

    if (auroraBackground != nullptr && auroraBackground->isVisible())
    {
        const float grAmount = juce::jlimit (0.0f, 1.0f, processorRef.currentGainReductionDb.load() / 20.0f);
        auroraBackground->setGainReductionAmount (grAmount);
    }
}

void SWATronix2AAudioProcessorEditor::paint (juce::Graphics& g)
{
    if (currentTheme == ThemeId::Aurora)
        return; // el fondo animado lo pinta AuroraBackground

    auto bounds = getLocalBounds().toFloat();

    switch (currentTheme)
    {
        case ThemeId::Photorealistic:
        {
            juce::ColourGradient grad (juce::Colour (0xff3a3a3a), 0.0f, 0.0f, juce::Colour (0xff1c1c1c), 0.0f, bounds.getHeight(), false);
            g.setGradientFill (grad);
            g.fillRect (bounds);

            for (auto corner : { juce::Point<float> (14, 14), juce::Point<float> (bounds.getWidth() - 14, 14),
                                  juce::Point<float> (14, bounds.getHeight() - 14), juce::Point<float> (bounds.getWidth() - 14, bounds.getHeight() - 14) })
            {
                g.setColour (juce::Colour (0xff888888));
                g.fillEllipse (juce::Rectangle<float> (10, 10).withCentre (corner));
                g.setColour (juce::Colours::black);
                g.drawLine (corner.x - 3, corner.y, corner.x + 3, corner.y, 1.0f);
            }

            g.setColour (juce::Colours::white.withAlpha (0.85f));
            g.setFont (juce::Font (16.0f, juce::Font::bold));
            g.drawText ("StreetWave Audio  /  SWA Teletronix 2A", bounds.removeFromTop (28.0f), juce::Justification::centred);
            break;
        }
        case ThemeId::Minimal:
        {
            g.fillAll (juce::Colour (0xffFAFAFA));
            g.setColour (juce::Colour (0xff2b2b2b));
            g.setFont (16.0f);
            g.drawText ("SWA Tronix 2A", bounds.removeFromTop (28.0f).toNearestInt(), juce::Justification::centred);
            break;
        }
        case ThemeId::Neon:
        {
            juce::ColourGradient grad (juce::Colour (0xff120a1f), 0.0f, 0.0f, juce::Colour (0xff05030a), 0.0f, bounds.getHeight(), false);
            g.setGradientFill (grad);
            g.fillRect (bounds);
            g.setColour (juce::Colour (0xffe0aaff));
            g.setFont (juce::Font (16.0f, juce::Font::bold));
            g.drawText ("SWA TRONIX 2A", bounds.removeFromTop (28.0f), juce::Justification::centred);
            break;
        }
        default: break;
    }
}

void SWATronix2AAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    if (auroraBackground != nullptr)
        auroraBackground->setBounds (bounds);

    if (infoPanel != nullptr)
        infoPanel->setBounds (bounds);

    auto top = bounds.removeFromTop (34);
    themeCombo.setBounds (top.removeFromRight (140).reduced (4));
    infoButton.setBounds (top.removeFromRight (30).reduced (2));

    auto presetArea = bounds.removeFromTop (30);
    presetBar.setBounds (presetArea.reduced (8, 2));

    auto controlsArea = bounds.reduced (16);
    auto meterArea = controlsArea.removeFromRight (controlsArea.getWidth() / 3);
    vuMeter.setBounds (meterArea.reduced (10));

    auto switchesArea = controlsArea.removeFromBottom (60);
    powerButton.setBounds (switchesArea.removeFromLeft (switchesArea.getWidth() / 3).reduced (8));
    modeButton.setBounds (switchesArea.removeFromLeft (switchesArea.getWidth() / 2).reduced (8));
    meterButton.setBounds (switchesArea.reduced (8));

    const auto knobWidth = controlsArea.getWidth() / 2;
    auto gainArea = controlsArea.removeFromLeft (knobWidth);
    auto peakArea = controlsArea;

    gainLabel.setBounds (gainArea.removeFromBottom (18));
    peakReductionLabel.setBounds (peakArea.removeFromBottom (18));

    gainKnob.setBounds (gainArea.reduced (20));
    peakReductionKnob.setBounds (peakArea.reduced (20));
}
