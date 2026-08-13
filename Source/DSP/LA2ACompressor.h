#pragma once
#include <juce_dsp/juce_dsp.h>
#include "OpticalCell.h"
#include "TubeStage.h"
#include "OutputTransformer.h"
#include "VUMeterBallistics.h"

enum class CompressorMode { Compress, Limit };

/**
 * Motor completo de la emulacion: celulas opticas L/R + valvulas de
 * entrada/salida + transformador de salida + oversampling en las
 * etapas no lineales para evitar aliasing.
 */
class LA2ACompressor
{
public:
    void prepare (const juce::dsp::ProcessSpec& spec);
    void reset();

    void setPeakReduction (float value01);
    void setGainDb (float gainDb);
    void setMode (CompressorMode mode);
    void setPower (bool isOn);

    void process (juce::AudioBuffer<float>& buffer);

    float getGainReductionDb() const { return smoothedGrDbForMeter; }
    float getOutputLevelDb() const { return outputLevelDbForMeter; }

private:
    double sampleRate = 44100.0;

    OpticalCell opticalCellL, opticalCellR;
    TubeStage inputTube, outputTube;
    OutputTransformer transformer;
    VUMeterBallistics vuBallistics;

    juce::dsp::Oversampling<float> oversampling { 2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    juce::LinearSmoothedValue<float> gainDbSmoothed;
    juce::LinearSmoothedValue<float> peakReductionSmoothed;

    CompressorMode mode = CompressorMode::Compress;
    bool powered = true;
    float powerRampState = 1.0f;

    float smoothedGrDbForMeter = 0.0f;
    float outputLevelDbForMeter = -60.0f;

    static float applyModeCurve (float grDb, CompressorMode mode);
};

