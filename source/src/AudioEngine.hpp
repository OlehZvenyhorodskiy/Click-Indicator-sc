#pragma once
#include "Macro.hpp"
#include <Geode/Geode.hpp>

class AudioEngine {
public:
    static AudioEngine* get();

    void playPressSound(SoundPack pack, float pitch, float volume);
    void playReleaseSound(SoundPack pack, float pitch, float volume);

private:
    std::string getPressFilename(SoundPack pack);
    std::string getReleaseFilename(SoundPack pack);
};