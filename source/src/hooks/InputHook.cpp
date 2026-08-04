#include "../Macro.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class $modify(ClickGuideInput, GJBaseGameLayer) {
    void handleButton(bool down, int button, bool player2) {
        GJBaseGameLayer::handleButton(down, button, player2);

        if (auto playLayer = typeinfo_cast<PlayLayer*>(this)) {
            // Can record input if recording mode is active
        }
    }
};