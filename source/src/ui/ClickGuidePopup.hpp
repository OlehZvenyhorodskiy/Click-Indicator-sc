#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

class ClickGuidePopup : public geode::Popup {
protected:
    int m_currentTab = 0;

    bool init(int initialTab);

    void buildTabs();
    void showTab(cocos2d::CCObject* sender);
    void initContent();

    void addToggle(const char* name, const char* settingKey, const char* description);
    void addNumber(const char* name, const char* settingKey, const char* description, double min, double max, double step, bool isFloat, const char* unit);
    void addSafeModeToggle();

    void drawPreview();
    void refreshCalib();
    void refreshHist();

    const char* gmName(int mode);

public:
    static ClickGuidePopup* create(int initialTab = 0);
    void update(float dt) override;
};