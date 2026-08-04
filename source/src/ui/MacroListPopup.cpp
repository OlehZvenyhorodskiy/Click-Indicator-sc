#include "MacroListPopup.hpp"
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

MacroListPopup* MacroListPopup::create(int levelID) {
    auto ret = new MacroListPopup();
    if (ret && ret->init(levelID)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MacroListPopup::init(int levelID) {
    if (!Popup::init(380.0f, 260.0f)) return false;
    m_levelID = levelID;
    this->setTitle("Download Macros");

    refetch();
    return true;
}

void MacroListPopup::refetch() {
    log::info("[Click Indicators] refetching macros for level {}", m_levelID);

    std::string url = fmt::format("https://clickindicatorsmod.com/api/mod/macros?level_id={}", m_levelID);

    auto res = web::WebRequest().getSync(url);
    if (!res.ok()) {
        log::error("[Click Indicators] failed to fetch macros from server");
        return;
    }

    auto jsonRes = res.json();
    if (!jsonRes) return;
    auto val = jsonRes.unwrap();

    m_macros.clear();
    if (val.isArray()) {
        for (const auto& item : val) {
            HyperMacro m;
            m.id = item["id"].asString().unwrapOr("");
            m.player = item["player"].asString().unwrapOr("Unknown");
            m.fps = static_cast<float>(item["fps"].asDouble().unwrapOr(240.0));
            m.click_count = item["clicks"].asInt().unwrapOr(0);
            m_macros.push_back(m);
        }
    }
    populateList();
}

void MacroListPopup::populateList() {
    if (m_scrollLayer) {
        m_scrollLayer->removeFromParent();
        m_scrollLayer = nullptr;
    }

    m_scrollLayer = ScrollLayer::create({ 340.0f, 180.0f });
    m_scrollLayer->setPosition({ (m_mainLayer->getContentSize().width - 340.0f) / 2, 20.0f });

    auto content = m_scrollLayer->m_contentLayer;
    float y = 0.0f;

    for (size_t i = 0; i < m_macros.size(); i++) {
        auto row = CCNode::create();
        row->setContentSize({ 330.0f, 30.0f });

        auto label = CCLabelBMFont::create(fmt::format("{} ({} clicks @ {:.0f} FPS)", m_macros[i].player, m_macros[i].click_count, m_macros[i].fps).c_str(), "chatFont.fnt");
        label->setAnchorPoint({ 0.0f, 0.5f });
        label->setPosition({ 10.0f, 15.0f });
        row->addChild(label);

        auto dlSprite = ButtonSprite::create("Get", 30, true, "goldFont.fnt", "GJ_button_01.png", 20.0f, 0.6f);
        auto dlBtn = CCMenuItemSpriteExtra::create(dlSprite, this, menu_selector(MacroListPopup::onDownloadRow));
        dlBtn->setTag(static_cast<int>(i));

        auto menu = CCMenu::create();
        menu->setPosition({ 290.0f, 15.0f });
        menu->addChild(dlBtn);
        row->addChild(menu);

        row->setPosition({ 5.0f, y });
        content->addChild(row);
        y += 35.0f;
    }

    content->setContentSize({ 340.0f, y });
    m_mainLayer->addChild(m_scrollLayer);
}

void MacroListPopup::onDownloadRow(CCObject* sender) {
    int idx = sender->getTag();
    if (idx < 0 || idx >= static_cast<int>(m_macros.size())) return;

    auto& m = m_macros[idx];
    log::info("[Click Indicators] downloading macro {} for level {}", m.id, m_levelID);

    std::string downloadUrl = fmt::format("https://clickindicatorsmod.com/api/mod/download?id={}", m.id);
    auto res = web::WebRequest().getSync(downloadUrl);
    if (res.ok()) {
        auto savePath = geode::Mod::get()->getSaveDir() / fmt::format("{}_{}.json", m_levelID, m.id);
        res.into(savePath);
        log::info("[Click Indicators] saved macro to {}", savePath.string());
        FLAlertLayer::create("Success", "Macro downloaded successfully!", "OK")->show();
        this->onClose(nullptr);
    } else {
        FLAlertLayer::create("Error", "Failed to download macro.", "OK")->show();
    }
}