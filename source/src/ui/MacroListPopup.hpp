#pragma once
#include "../Macro.hpp"
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

class MacroListPopup : public geode::Popup {
protected:
    int m_levelID = 0;
    std::vector<HyperMacro> m_macros;
    geode::ScrollLayer* m_scrollLayer = nullptr;

    bool init(int levelID);

    void refetch();
    void populateList();
    void onDownloadRow(cocos2d::CCObject* sender);

public:
    static MacroListPopup* create(int levelID);
};