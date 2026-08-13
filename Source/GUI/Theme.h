#pragma once

enum class ThemeId
{
    Photorealistic = 0,
    Minimal,
    Neon,
    Aurora
};

inline const char* themeName (ThemeId t)
{
    switch (t)
    {
        case ThemeId::Photorealistic: return "Fotorrealista";
        case ThemeId::Minimal:        return "Minimalista";
        case ThemeId::Neon:           return "Neon Morado";
        case ThemeId::Aurora:         return "Aurora";
        default:                      return "";
    }
}
