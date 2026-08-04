#include "MacroParser.hpp"
#include <fstream>
#include <matjson.hpp>

using namespace geode::prelude;

bool isMacroExt(const std::filesystem::path& path) {
    auto ext = path.extension().string();
    for (auto& c : ext) c = tolower(c);
    return ext == ".json" || ext == ".mhr" || ext == ".slc" || ext == ".gdr" || ext == ".xd" || ext == ".cb";
}

static bool parseJsonMacro(const std::string& content, std::vector<Action>& outActions, float& outFps) {
    auto res = matjson::Value::parse(content);
    if (!res) return false;
    auto val = res.unwrap();

    if (val.contains("fps")) {
        outFps = static_cast<float>(val["fps"].asDouble().unwrapOr(240.0));
    } else if (val.contains("framerate")) {
        outFps = static_cast<float>(val["framerate"].asDouble().unwrapOr(240.0));
    } else {
        outFps = 240.0f;
    }

    auto parseInputs = [&](const matjson::Value& arr) {
        if (!arr.isArray()) return;
        for (const auto& item : arr) {
            Action act;
            if (item.contains("frame")) {
                act.frame = item["frame"].asInt().unwrapOr(0);
                act.time = static_cast<double>(act.frame) / outFps;
            } else if (item.contains("time")) {
                act.time = item["time"].asDouble().unwrapOr(0.0);
                act.frame = static_cast<int>(act.time * outFps);
            }
            if (item.contains("btn")) {
                act.button = item["btn"].asInt().unwrapOr(1);
            }
            if (item.contains("down")) {
                act.push = item["down"].asBool().unwrapOr(false);
            } else if (item.contains("press")) {
                act.push = item["press"].asBool().unwrapOr(false);
            }
            if (item.contains("p2")) {
                act.player2 = item["p2"].asBool().unwrapOr(false);
            }
            if (item.contains("xpos")) {
                act.x_pos = static_cast<float>(item["xpos"].asDouble().unwrapOr(0.0));
            }
            outActions.push_back(act);
        }
    };

    if (val.contains("inputs")) {
        parseInputs(val["inputs"]);
    } else if (val.contains("events")) {
        parseInputs(val["events"]);
    } else if (val.contains("actions")) {
        parseInputs(val["actions"]);
    }

    return !outActions.empty();
}

static bool parseMhrMacro(const std::vector<uint8_t>& bytes, std::vector<Action>& outActions, float& outFps) {
    if (bytes.size() < 12) return false;
    
    float tps = *reinterpret_cast<const float*>(bytes.data() + 4);
    if (tps < 30.0f || tps > 10000.0f) {
        log::warn("[Click Indicators] .mhr declares {} ticks/sec, out of range - assuming 240", tps);
        tps = 240.0f;
    }
    outFps = tps;

    size_t offset = 12;
    while (offset + 12 <= bytes.size()) {
        uint32_t frame = *reinterpret_cast<const uint32_t*>(bytes.data() + offset);
        uint8_t down = bytes[offset + 4];
        uint8_t p2 = bytes[offset + 5];
        uint8_t btn = bytes[offset + 6];
        
        Action act;
        act.frame = frame;
        act.time = static_cast<double>(frame) / tps;
        act.push = (down != 0);
        act.player2 = (p2 != 0);
        act.button = btn != 0 ? btn : 1;
        outActions.push_back(act);
        offset += 12;
    }
    return !outActions.empty();
}

static bool parseSlcMacro(const std::vector<uint8_t>& bytes, std::vector<Action>& outActions, float& outFps) {
    if (bytes.size() < 8) return false;
    float tps = *reinterpret_cast<const float*>(bytes.data());
    if (tps < 30.0f || tps > 10000.0f) {
        log::warn("[Click Indicators] .slc declares {} ticks/sec, out of range - assuming 240, so the timing will be wrong", tps);
        tps = 240.0f;
    }
    outFps = tps;

    size_t offset = 8;
    while (offset + 16 <= bytes.size()) {
        float x = *reinterpret_cast<const float*>(bytes.data() + offset);
        uint32_t frame = *reinterpret_cast<const uint32_t*>(bytes.data() + offset + 4);
        uint8_t down = bytes[offset + 8];
        uint8_t p2 = bytes[offset + 9];
        
        Action act;
        act.x_pos = x;
        act.frame = frame;
        act.time = static_cast<double>(frame) / tps;
        act.push = (down != 0);
        act.player2 = (p2 != 0);
        outActions.push_back(act);
        offset += 16;
    }
    return !outActions.empty();
}

bool parseMacroFile(const std::filesystem::path& path, std::vector<Action>& outActions, float& outFps) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (bytes.empty()) return false;

    auto ext = path.extension().string();
    for (auto& c : ext) c = tolower(c);

    if (ext == ".json") {
        std::string str(bytes.begin(), bytes.end());
        return parseJsonMacro(str, outActions, outFps);
    } else if (ext == ".mhr") {
        return parseMhrMacro(bytes, outActions, outFps);
    } else if (ext == ".slc") {
        return parseSlcMacro(bytes, outActions, outFps);
    } else {
        std::string str(bytes.begin(), bytes.end());
        if (parseJsonMacro(str, outActions, outFps)) return true;
        return parseMhrMacro(bytes, outActions, outFps);
    }
}