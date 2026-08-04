#include "AudioEngine.hpp"

using namespace geode::prelude;

AudioEngine* AudioEngine::get() {
    static AudioEngine instance;
    return &instance;
}

std::string AudioEngine::getPressFilename(SoundPack pack) {
    switch (pack) {
        case SoundPack::Beep: return "press_beep.wav";
        case SoundPack::Wood: return "press_wood.wav";
        case SoundPack::Snap: return "press_snap.wav";
        default: return "press.wav";
    }
}

std::string AudioEngine::getReleaseFilename(SoundPack pack) {
    switch (pack) {
        case SoundPack::Beep: return "release_beep.wav";
        case SoundPack::Wood: return "release_wood.wav";
        case SoundPack::Snap: return "release_snap.wav";
        default: return "release.wav";
    }
}

void AudioEngine::playPressSound(SoundPack pack, float pitch, float volume) {
    auto soundFile = Mod::get()->getResourcesDir() / "jackz.click-indicators" / getPressFilename(pack);
    FMODAudioEngine::sharedEngine()->playEffect(soundFile.string(), pitch, 0.0f, volume);
}

void AudioEngine::playReleaseSound(SoundPack pack, float pitch, float volume) {
    auto soundFile = Mod::get()->getResourcesDir() / "jackz.click-indicators" / getReleaseFilename(pack);
    FMODAudioEngine::sharedEngine()->playEffect(soundFile.string(), pitch, 0.0f, volume);
}