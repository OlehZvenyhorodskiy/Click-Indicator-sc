#pragma once
#include <string>
#include <vector>
#include <Geode/Geode.hpp>

struct Action {
    double time = 0.0;
    float x_pos = 0.0f;
    int frame = 0;
    bool push = false; // true = press, false = release
    bool player2 = false;
    int button = 1; // 1 = jump/primary
};

struct HyperMacro {
    std::string id;
    int level_id = 0;
    std::string player;
    float fps = 240.0f;
    float ratio = 1.0f;
    float end_x = 0.0f;
    double duration = 0.0;
    int click_count = 0;
    std::vector<Action> actions;
};

enum class IndicatorStyle {
    Ring,
    Classic,
    Converge,
    Pulse,
    Highway
};

enum class SoundPack {
    Click,
    Beep,
    Wood,
    Snap
};

inline IndicatorStyle parseIndicatorStyle(const std::string& str) {
    if (str == "classic") return IndicatorStyle::Classic;
    if (str == "converge") return IndicatorStyle::Converge;
    if (str == "pulse") return IndicatorStyle::Pulse;
    if (str == "highway") return IndicatorStyle::Highway;
    return IndicatorStyle::Ring;
}

inline SoundPack parseSoundPack(const std::string& str) {
    if (str == "beep") return SoundPack::Beep;
    if (str == "wood") return SoundPack::Wood;
    if (str == "snap") return SoundPack::Snap;
    return SoundPack::Click;
}