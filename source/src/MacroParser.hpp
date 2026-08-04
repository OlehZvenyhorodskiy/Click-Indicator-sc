#pragma once
#include "Macro.hpp"
#include <filesystem>
#include <vector>

bool isMacroExt(const std::filesystem::path& path);
bool parseMacroFile(const std::filesystem::path& path, std::vector<Action>& outActions, float& outFps);