#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(ClickIndicatorsMenu, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        std::string ver = Mod::get()->getVersion().toVString();
        log::info("[Click Indicators] Mod initialized: {}", ver);

        return true;
    }
};