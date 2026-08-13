# SWA Tronix 2A — StreetWave Audio

Emulacion de amplificador de nivelacion optico (leveling amplifier), formato **VST3**,
desarrollada con **JUCE 7 + CMake**. Inspirada en el comportamiento del clasico
compresor optico de la era vintage (celula T4, valvulas, transformador de salida).

## Estructura del proyecto

```
SWA-Tronix-2A/
├── CMakeLists.txt              # build con JUCE via FetchContent
├── .github/workflows/build.yml # compila VST3 en macOS y Linux automaticamente
└── Source/
    ├── PluginProcessor.h/.cpp  # parametros (APVTS), audio callback, estado
    ├── PluginEditor.h/.cpp     # UI: ensambla los 4 temas, presets, meter, info
    ├── DSP/
    │   ├── OpticalCell.h/.cpp        # modelo de la celula T4 (ataque/release no lineal)
    │   ├── TubeStage.h/.cpp          # saturacion de valvulas (armonicos pares)
    │   ├── OutputTransformer.h/.cpp  # coloracion del transformador de salida
    │   ├── VUMeterBallistics.h/.cpp  # integracion ~300ms del VU meter
    │   └── LA2ACompressor.h/.cpp     # motor completo + oversampling
    ├── GUI/
    │   ├── ThemeLookAndFeels.h/.cpp  # los 4 temas visuales (LookAndFeel)
    │   ├── VUMeterComponent.h/.cpp   # aguja analogica / barra / LEDs / aurora
    │   ├── AuroraBackground.h/.cpp   # fondo animado del Tema 4
    │   ├── InfoPanel.h/.cpp          # panel modal de informacion
    │   └── PresetBar.h/.cpp          # selector de presets
    └── Presets/
        ├── PresetManager.h/.cpp      # carga/guardado de presets (fabrica + usuario)
        └── FactoryPresets.h/.cpp     # los 12 presets de fabrica

```

## Compilar

### Opcion A — GitHub Actions (recomendado, sin instalar nada)

1. Sube esta carpeta a un repositorio de GitHub.
2. El workflow `.github/workflows/build.yml` se dispara automaticamente en cada
   push a `main` (o manualmente desde la pestana "Actions" → "Run workflow").
3. Compila para **macOS** y **Linux** y deja el `.vst3` descargable como artefacto
   de la ejecucion (pestana Actions → tu run → Artifacts).

### Opcion B — Local (macOS / Linux)

Requiere CMake ≥ 3.22, un compilador C++17 y conexion a internet (JUCE se
descarga automaticamente via `FetchContent` la primera vez).

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
# El .vst3 resultante queda dentro de build/SWATronix2A_artefacts/Release/VST3/
```

En Linux necesitas antes las dependencias de JUCE (ver el paso "Instalar
dependencias" del workflow de GitHub Actions para la lista completa).

## Notas sobre el modelo DSP

El motor (`OpticalCell`, `TubeStage`, `OutputTransformer`) es una aproximacion
de envolvente e implementacion propia inspirada en el comportamiento
documentado publicamente del tipo de compresor optico que pediste emular
(ataque ~10ms fijo, release en dos tramos, rodilla suave, saturacion de
valvulas, coloracion del transformador) — **no** es una traduccion de ningun
esquematico ni codigo propietario. Es un punto de partida solido y ya
funcional a nivel de arquitectura, pero como con cualquier emulacion
analogica, afinar el "feel" exacto (curvas, tiempos, cantidad de saturacion)
normalmente requiere iteracion por oido comparando con el hardware o
referencias de audio real. Los comentarios en el codigo indican donde tocar
cada parametro (`OpticalCell.cpp` para los tiempos de ataque/release,
`TubeStage.cpp`/`OutputTransformer.cpp` para la cantidad de color).

## Estado del proyecto / siguientes pasos sugeridos

Este es un scaffold completo y compilable (arquitectura, DSP, 4 temas, presets,
ventana redimensionable, panel de info) escrito integramente para este pedido,
pero **no ha sido compilado ni probado en un DAW real** porque el entorno donde
se genero no tiene acceso a red ni al SDK de JUCE. Al compilarlo por primera vez
es normal (y esperable en un proyecto JUCE de este tamano) tener que resolver
algun detalle menor de API segun la version exacta de JUCE que use CMake. Ideas
para seguir puliendolo:

- Sustituir las texturas proceduales del Tema 1 por assets PNG/SVG reales si
  quieres un acabado 100% fotografico.
- Anadir un flujo de guardado/carga de presets de usuario desde la propia UI
  (ya esta el backend en `PresetManager`, falta un dialogo de texto para el nombre).
- Anadir formato AU si en el futuro quieres soporte nativo en Logic/GarageBand
  (JUCE lo permite agregando `AU` a `FORMATS` en `CMakeLists.txt`, solo en macOS).
