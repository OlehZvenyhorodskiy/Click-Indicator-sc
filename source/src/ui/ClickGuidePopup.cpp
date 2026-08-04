#include "ClickGuidePopup.hpp"

using namespace geode::prelude;

ClickGuidePopup* ClickGuidePopup::create(int initialTab) {
    auto ret = new ClickGuidePopup();
    if (ret && ret->init(initialTab)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ClickGuidePopup::init(int initialTab) {
    if (!Popup::init(420.0f, 280.0f)) return false;

    this->setTitle("Click Indicators");

    m_currentTab = initialTab;
    buildTabs();
    initContent();

    this->scheduleUpdate();
    return true;
}

void ClickGuidePopup::buildTabs() {
    static const char* NAMES[] = { "General", "Visuals", "Sound", "Gamemodes" };

    auto menu = CCMenu::create();
    menu->setPosition({ m_mainLayer->getContentSize().width / 2, m_mainLayer->getContentSize().height - 45.0f });

    for (int i = 0; i < 4; i++) {
        auto label = CCLabelBMFont::create(NAMES[i], "bigFont.fnt");
        label->setScale(0.5f);

        auto btn = CCMenuItemSpriteExtra::create(
            label,
            this,
            menu_selector(ClickGuidePopup::showTab)
        );
        btn->setTag(i);
        menu->addChild(btn);
    }
    menu->setLayout(RowLayout::create()->setGap(15.0f));
    m_mainLayer->addChild(menu);
}

void ClickGuidePopup::showTab(CCObject* sender) {
    if (sender) {
        m_currentTab = sender->getTag();
    }
    log::info("[Click Indicators] switched to tab {}", m_currentTab);
}

void ClickGuidePopup::initContent() {
    addSafeModeToggle();
}

void ClickGuidePopup::addToggle(const char* name, const char* settingKey, const char* description) {
}

void ClickGuidePopup::addNumber(const char* name, const char* settingKey, const char* description, double min, double max, double step, bool isFloat, const char* unit) {
}

void ClickGuidePopup::addSafeModeToggle() {
    bool currentSafeMode = Mod::get()->getSettingValue<bool>("safe-mode");
    auto label = CCLabelBMFont::create("Safe Mode", "goldFont.fnt");
    label->setScale(0.6f);
    label->setPosition({ 70.0f, m_mainLayer->getContentSize().height - 80.0f });
    m_mainLayer->addChild(label);
}

void ClickGuidePopup::drawPreview() {
}

void ClickGuidePopup::refreshCalib() {
}

void ClickGuidePopup::refreshHist() {
}

const char* ClickGuidePopup::gmName(int mode) {
    static const char* N[] = { "Cube", "Ship", "Ball", "UFO", "Wave", "Robot", "Spider", "Swing" };
    if (mode >= 0 && mode < 8) return N[mode];
    return "Unknown";
}

void ClickGuidePopup::update(float dt) {
}