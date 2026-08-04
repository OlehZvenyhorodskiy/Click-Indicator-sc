#include "../ui/MacroListPopup.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(ClickGuideLevelInfo, LevelInfoLayer) {
    void onEnterTransitionDidFinish() {
        LevelInfoLayer::onEnterTransitionDidFinish();

        if (!m_level || m_level->m_levelID <= 0) return;

        auto menu = this->getChildByID("left-side-menu");
        if (!menu) menu = this->getChildByID("right-side-menu");

        if (menu) {
            auto btnSprite = ButtonSprite::create("Macros", 30, true, "goldFont.fnt", "GJ_button_02.png", 30.0f, 0.7f);
            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(ClickGuideLevelInfo::onOpenMacroList)
            );
            btn->setID("jackz.click-indicators/cg-fetch-btn");
            menu->addChild(btn);
            menu->updateLayout();
        }
    }

    void onOpenMacroList(CCObject* sender) {
        if (m_level && m_level->m_levelID > 0) {
            MacroListPopup::create(m_level->m_levelID.value())->show();
        }
    }
};