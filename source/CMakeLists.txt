cmake_minimum_required(VERSION 3.25)

if (DEFINED ENV{GEODE_SDK})
    file(TO_CMAKE_PATH "$ENV{GEODE_SDK}" GEODE_SDK_CLEAN)
else()
    message(FATAL_ERROR "Set GEODE_SDK environment variable")
endif()

list(APPEND CMAKE_MODULE_PATH "${GEODE_SDK_CLEAN}/cmake")
include(GeodeFile)

project(ClickIndicators VERSION 1.0.19 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("${GEODE_SDK_CLEAN}" "${CMAKE_CURRENT_BINARY_DIR}/geode-sdk")

add_library(
    jackz.click-indicators SHARED
    src/main.cpp
    src/MacroParser.cpp
    src/AudioEngine.cpp
    src/CueRenderer.cpp
    src/hooks/PlayLayerHook.cpp
    src/hooks/InputHook.cpp
    src/hooks/PauseLayerHook.cpp
    src/hooks/LevelInfoHook.cpp
    src/hooks/MenuLayerHook.cpp
    src/ui/ClickGuidePopup.cpp
    src/ui/MacroListPopup.cpp
)

setup_geode_mod(jackz.click-indicators)
