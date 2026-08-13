#include "FactoryPresets.h"

std::vector<FactoryPreset> FactoryPresets::getAll()
{
    return {
        { "Vocal Suave",           2.0f, 0.35f, 0 },
        { "Vocal Radial",          4.0f, 0.55f, 0 },
        { "Bajo Electrico",        3.0f, 0.45f, 0 },
        { "Bus de Bateria",        1.5f, 0.30f, 0 },
        { "Master Sutil",          0.5f, 0.20f, 0 },
        { "Limitador de Picos",    2.0f, 0.70f, 1 },
        { "Guitarra Acustica",     1.5f, 0.25f, 0 },
        { "Voz Hablada / Podcast", 3.5f, 0.50f, 0 },
        { "Compresion Paralela",   6.0f, 0.85f, 0 },
        { "Calidez Vintage",       2.5f, 0.40f, 0 },
        { "Bus de Sintetizadores", 2.0f, 0.35f, 0 },
        { "Todo Terreno",          2.0f, 0.40f, 0 }
    };
}
