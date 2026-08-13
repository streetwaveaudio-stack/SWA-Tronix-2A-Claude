#pragma once

/** Aproxima la balistica de un VU meter estandar: ~300ms para alcanzar el 99% de un escalon. */
class VUMeterBallistics
{
public:
    void prepare (double sampleRate);
    void reset();
    float process (float inputLevelLinear);
    float getCurrentDb() const { return currentDb; }

private:
    double sr = 44100.0;
    float coeff = 0.0f;
    float smoothedLevel = 0.0f;
    float currentDb = -60.0f;
};
