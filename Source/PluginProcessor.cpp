#include "PluginProcessor.h"
#include "PluginEditor.h"

SWATronix2AAudioProcessor::SWATronix2AAudioProcessor()
    : AudioProcessor (BusesProperties()
                        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMETERS", createParameterLayout()),
      presetManager (apvts)
{
}

SWATronix2AAudioProcessor::~SWATronix2AAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout SWATronix2AAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_GAIN, 1 }, "Gain",
        juce::NormalisableRange<float> (-20.0f, 40.0f, 0.1f), 0.0f,
        juce::AudioParameterFloatAttributes().withLabel ("dB")));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_PEAK_REDUCTION, 1 }, "Peak Reduction",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.4f));

    params.push_back (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { PARAM_MODE, 1 }, "Mode",
        juce::StringArray { "Compress", "Limit" }, 0));

    params.push_back (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { PARAM_METER, 1 }, "Meter",
        juce::StringArray { "Output", "Gain Reduction" }, 1));

    params.push_back (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { PARAM_POWER, 1 }, "Power", true));

    return { params.begin(), params.end() };
}

void SWATronix2AAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32) samplesPerBlock;
    spec.numChannels = (juce::uint32) getTotalNumOutputChannels();

    compressorEngine.prepare (spec);
}

void SWATronix2AAudioProcessor::releaseResources() {}

bool SWATronix2AAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo()
        || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono();
}

void SWATronix2AAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    compressorEngine.setGainDb (*apvts.getRawParameterValue (PARAM_GAIN));
    compressorEngine.setPeakReduction (*apvts.getRawParameterValue (PARAM_PEAK_REDUCTION));
    compressorEngine.setMode (apvts.getRawParameterValue (PARAM_MODE)->load() > 0.5f
                                   ? CompressorMode::Limit : CompressorMode::Compress);
    compressorEngine.setPower (*apvts.getRawParameterValue (PARAM_POWER) > 0.5f);

    compressorEngine.process (buffer);

    currentGainReductionDb.store (compressorEngine.getGainReductionDb());
    currentOutputLevelDb.store (compressorEngine.getOutputLevelDb());
}

juce::AudioProcessorEditor* SWATronix2AAudioProcessor::createEditor()
{
    return new SWATronix2AAudioProcessorEditor (*this);
}

void SWATronix2AAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    if (auto xml = state.createXml())
    {
        xml->setAttribute ("selectedTheme", selectedThemeIndex.load());
        copyXmlToBinary (*xml, destData);
    }
}

void SWATronix2AAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
    {
        selectedThemeIndex.store (xml->getIntAttribute ("selectedTheme", 0));
        apvts.replaceState (juce::ValueTree::fromXml (*xml));
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SWATronix2AAudioProcessor();
}
