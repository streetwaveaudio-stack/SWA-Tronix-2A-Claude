#pragma once
#include <juce_dsp/juce_dsp.h>
#include <cmath>

/**
 * Modelo aproximado de la celula optica T4 del LA-2A: un panel
 * electroluminiscente (EL) que se ilumina segun la senal de programa,
 * acoplado a una fotorresistencia (LDR) cuya resistencia determina la
 * cantidad de reduccion de ganancia.
 *
 * No es una simulacion de circuito a nivel de componente; es un modelo
 * de envolvente de doble etapa afinado para reproducir el comportamiento
 * caracteristico documentado del LA-2A:
 *  - Ataque rapido (~10 ms), practicamente fijo.
 *  - Release en dos tramos: recuperacion rapida inicial (~60 ms, ~50-60%)
 *    seguida de una cola lenta cuya duracion depende de la "exposicion"
 *    previa de la celula (memoria de fosforo), pudiendo durar varios segundos.
 *  - Rodilla suave (soft-knee) en la conversion nivel -> reduccion.
 */
class OpticalCell
{
public:
    void prepare (double sampleRate);
    void reset();

    // sidechainLevelDb: nivel en dB de la senal que excita la celula.
    // peakReductionAmount: 0..1, mapea el control "Peak Reduction" del panel.
    // Devuelve la reduccion de ganancia instantanea en dB (positivo = mas reduccion).
    float process (float sidechainLevelDb, float peakReductionAmount);

    float getExposure() const { return exposure; }

private:
    double sr = 44100.0;

    float brightness = 0.0f;
    float exposure = 0.0f;
    float peakBrightnessAtReleaseStart = 0.0f;
    float lastGrDb = 0.0f;
    bool wasAttacking = false;

    float attackCoeff = 0.0f;
    float releaseFastCoeff = 0.0f;
    float releaseSlowCoeff = 0.0f;
    float exposureAttackCoeff = 0.0f;
    float exposureReleaseCoeff = 0.0f;

    static float msToCoeff (float ms, double sampleRate);
};
