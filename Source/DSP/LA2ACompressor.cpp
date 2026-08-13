#include "LA2ACompressor.h"
#include <vector>

void LA2ACompressor::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;

    opticalCellL.prepare (sampleRate);
    opticalCellR.prepare (sampleRate);
    inputTube.prepare (sampleRate);
    outputTube.prepare (sampleRate);
    transformer.prepare (sampleRate);
    vuBallistics.prepare (sampleRate);

    oversampling.initProcessing ((size_t) spec.maximumBlockSize);
    oversampling.reset();

    gainDbSmoothed.reset (sampleRate, 0.02);
    peakReductionSmoothed.reset (sampleRate, 0.02);
    gainDbSmoothed.setCurrentAndTargetValue (0.0f);
    peakReductionSmoothed.setCurrentAndTargetValue (0.4f);

    reset();
}

void LA2ACompressor::reset()
{
    opticalCellL.reset();
    opticalCellR.reset();
    inputTube.reset();
    outputTube.reset();
    transformer.reset();
    vuBallistics.reset();
    oversampling.reset();
}

void LA2ACompressor::setPeakReduction (float value01) { peakReductionSmoothed.setTargetValue (juce::jlimit (0.0f, 1.0f, value01)); }
void LA2ACompressor::setGainDb (float gainDb)          { gainDbSmoothed.setTargetValue (gainDb); }
void LA2ACompressor::setMode (CompressorMode m)        { mode = m; }
void LA2ACompressor::setPower (bool isOn)              { powered = isOn; }

float LA2ACompressor::applyModeCurve (float grDb, CompressorMode mode)
{
    // El propio comportamiento de la celula ya aporta un ratio suave y variable.
    // En modo Limit se acentua para un comportamiento mucho mas orientado a picos.
    return (mode == CompressorMode::Limit) ? grDb * 1.8f : grDb;
}

void LA2ACompressor::process (juce::AudioBuffer<float>& buffer)
{
    const int numSamples = buffer.getNumSamples();

    // Rampa de "calentamiento" al encender/apagar (guino a las valvulas reales)
    if (powered)
        powerRampState = juce::jmin (1.0f, powerRampState + (float) (numSamples / (sampleRate * 0.5)));
    else
        powerRampState = juce::jmax (0.0f, powerRampState - (float) (numSamples / (sampleRate * 0.3)));

    inputTube.process (buffer, 0.15f * powerRampState);

    auto block = juce::dsp::AudioBlock<float> (buffer);
    auto oversampledBlock = oversampling.processSamplesUp (block);

    float peakGrDbThisBlock = 0.0f;

    for (size_t ch = 0; ch < oversampledBlock.getNumChannels() && ch < 2; ++ch)
    {
        auto* data = oversampledBlock.getChannelPointer (ch);
        auto& cell = (ch == 0) ? opticalCellL : opticalCellR;

        for (size_t i = 0; i < oversampledBlock.getNumSamples(); ++i)
        {
            const float peakRed = peakReductionSmoothed.getNextValue();
            const float inLevelDb = juce::Decibels::gainToDecibels (std::abs (data[i]), -100.0f);

            float grDb = cell.process (inLevelDb, peakRed) * powerRampState;
            grDb = applyModeCurve (grDb, mode);
            peakGrDbThisBlock = juce::jmax (peakGrDbThisBlock, grDb);

            data[i] *= juce::Decibels::decibelsToGain (-grDb);
        }
    }

    smoothedGrDbForMeter = smoothedGrDbForMeter * 0.9f + peakGrDbThisBlock * 0.1f;

    oversampling.processSamplesDown (block);

    // Ganancia de compensacion (makeup)
    std::vector<float> makeupGains ((size_t) numSamples);
    for (int i = 0; i < numSamples; ++i)
        makeupGains[(size_t) i] = juce::Decibels::decibelsToGain (gainDbSmoothed.getNextValue());

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getWritePointer (ch);
        for (int i = 0; i < numSamples; ++i)
            data[i] *= makeupGains[(size_t) i];
    }

    outputTube.process (buffer, 0.25f * powerRampState);
    transformer.process (buffer, powerRampState);

    float peakOut = 0.0f;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        peakOut = juce::jmax (peakOut, buffer.getMagnitude (ch, 0, numSamples));

    outputLevelDbForMeter = vuBallistics.process (peakOut);
}
