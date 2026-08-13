#include "OpticalCell.h"

float OpticalCell::msToCoeff (float ms, double sampleRate)
{
    if (ms <= 0.0f) return 0.0f;
    return std::exp (-1.0f / (0.001f * ms * (float) sampleRate));
}

void OpticalCell::prepare (double sampleRate)
{
    sr = sampleRate;
    attackCoeff          = msToCoeff (10.0f, sr);     // ataque ~10ms, practicamente fijo
    releaseFastCoeff     = msToCoeff (60.0f, sr);      // primer tramo de release
    releaseSlowCoeff      = msToCoeff (4000.0f, sr);    // cola lenta base (hasta varios s)
    exposureAttackCoeff  = msToCoeff (200.0f, sr);
    exposureReleaseCoeff = msToCoeff (3000.0f, sr);
    reset();
}

void OpticalCell::reset()
{
    brightness = 0.0f;
    exposure = 0.0f;
    peakBrightnessAtReleaseStart = 0.0f;
    lastGrDb = 0.0f;
    wasAttacking = false;
}

float OpticalCell::process (float sidechainLevelDb, float peakReductionAmount)
{
    // El control "Peak Reduction" no es un threshold clasico: determina
    // cuanta senal llega a excitar la celula optica.
    const float drive = juce::jmap (peakReductionAmount, 0.0f, 1.0f, -20.0f, 24.0f);
    const float over = sidechainLevelDb + drive;

    // Rodilla suave de ~6 dB
    const float kneeWidth = 6.0f;
    float softened;
    if (over <= -kneeWidth * 0.5f)       softened = 0.0f;
    else if (over >= kneeWidth * 0.5f)   softened = over;
    else                                  softened = std::pow (over + kneeWidth * 0.5f, 2.0f) / (2.0f * kneeWidth);

    const float targetBrightness = juce::jlimit (0.0f, 1.0f, softened / 30.0f);

    // Memoria de fosforo: se acumula con exposicion sostenida y ralentiza
    // aun mas el release lento subsecuente.
    const float expCoeff = (targetBrightness > exposure) ? exposureAttackCoeff : exposureReleaseCoeff;
    exposure = targetBrightness + (exposure - targetBrightness) * expCoeff;

    const float dynamicSlowCoeff = juce::jlimit (releaseFastCoeff, 0.9999f,
                                                  releaseSlowCoeff + (0.9999f - releaseSlowCoeff) * exposure);

    const bool attacking = targetBrightness >= brightness;

    if (attacking)
    {
        if (! wasAttacking)
            peakBrightnessAtReleaseStart = brightness;

        brightness = targetBrightness + (brightness - targetBrightness) * attackCoeff;
    }
    else
    {
        if (wasAttacking)
            peakBrightnessAtReleaseStart = brightness;

        const float span = juce::jmax (0.0001f, peakBrightnessAtReleaseStart);
        const float recovered = (span - brightness) / span;

        // Primer ~55% de la caida: tramo rapido. Resto: cola lenta dependiente de exposicion.
        const float coeff = (recovered < 0.55f) ? releaseFastCoeff : dynamicSlowCoeff;
        brightness = targetBrightness + (brightness - targetBrightness) * coeff;
    }

    wasAttacking = attacking;
    brightness = juce::jlimit (0.0f, 1.0f, brightness);
    lastGrDb = brightness * 30.0f;

    return lastGrDb;
}
