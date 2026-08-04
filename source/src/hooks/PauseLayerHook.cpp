#include "../ui/ClickGuidePopup.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(ClickGuidePause, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto menu = this->getChildByID("right-side-menu");
        if (!menu) menu = this->getChildByID("left-side-menu");

        if (menu && Mod::get()->getSettingValue<bool>("record-button")) {
            auto btnSprite = ButtonSprite::create("Guide", 30, true, "goldFont.fnt", "GJ_button_01.png", 30.0f, 0.7f);
            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(ClickGuidePause::onOpenGuideSettings)
            );
            btn->setID("jackz.click-indicators/cg-pause-btn");
            menu->addChild(btn);
            menu->updateLayout();
        }
    }

    void onOpenGuideSettings(CCObject* sender) {
        ClickGuidePopup::create(0)->show();
    }
};