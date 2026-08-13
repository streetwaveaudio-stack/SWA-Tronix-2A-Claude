cmake_minimum_required(VERSION 3.22)

project(SWATronix2A VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        7.0.12
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(SWATronix2A
    COMPANY_NAME "StreetWave Audio"
    PLUGIN_MANUFACTURER_CODE Swav
    PLUGIN_CODE Sw2a
    FORMATS VST3
    PRODUCT_NAME "SWA Tronix 2A"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    VST3_CATEGORIES "Fx" "Dynamics"
    COPY_PLUGIN_AFTER_BUILD TRUE
)

juce_generate_juce_header(SWATronix2A)

target_sources(SWATronix2A PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/DSP/OpticalCell.cpp
    Source/DSP/TubeStage.cpp
    Source/DSP/OutputTransformer.cpp
    Source/DSP/VUMeterBallistics.cpp
    Source/DSP/LA2ACompressor.cpp
    Source/GUI/ThemeLookAndFeels.cpp
    Source/GUI/VUMeterComponent.cpp
    Source/GUI/InfoPanel.cpp
    Source/GUI/PresetBar.cpp
    Source/GUI/AuroraBackground.cpp
    Source/Presets/PresetManager.cpp
    Source/Presets/FactoryPresets.cpp
)

target_compile_definitions(SWATronix2A PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(SWATronix2A PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_gui_extra
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
