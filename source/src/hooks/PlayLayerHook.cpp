#include "../Macro.hpp"
#include "../MacroParser.hpp"
#include "../AudioEngine.hpp"
#include "../CueRenderer.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(ClickGuidePlayLayer, PlayLayer) {
    struct Fields {
        std::vector<Action> m_actions;
        float m_macroFps = 240.0f;
        size_t m_currentActionIdx = 0;
        cocos2d::CCDrawNode* m_drawNode = nullptr;
        bool m_hasLoadedMacro = false;
        bool m_recording = false;
        std::vector<Action> m_recordedActions;
        double m_levelTime = 0.0;
        int m_lastJudgedFrame = -1;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        m_fields->m_drawNode = cocos2d::CCDrawNode::create();
        m_fields->m_drawNode->setID("jackz.click-indicators/ci-overlay");
        m_fields->m_drawNode->setZOrder(100);
        this->m_objectLayer->addChild(m_fields->m_drawNode);

        // Try loading local macro for level
        if (level && level->m_levelID > 0) {
            auto macroPath = Mod::get()->getSaveDir() / fmt::format("{}.json", level->m_levelID.value());
            if (std::filesystem::exists(macroPath)) {
                if (parseMacroFile(macroPath, m_fields->m_actions, m_fields->m_macroFps)) {
                    m_fields->m_hasLoadedMacro = true;
                    log::info("[Click Indicators] {} chosen: {} clicks", macroPath.string(), m_fields->m_actions.size());
                }
            }
        }

        return true;
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        if (!Mod::get()->getSettingValue<bool>("enabled") || !m_fields->m_drawNode) return;

        m_fields->m_drawNode->clear();
        m_fields->m_levelTime += dt;

        if (!m_fields->m_hasLoadedMacro || m_fields->m_actions.empty()) return;

        double currentTime = m_fields->m_levelTime;
        double lookahead = Mod::get()->getSettingValue<double>("lookahead");
        double lead = Mod::get()->getSettingValue<double>("lead");
        double adjustedTime = currentTime + lead;

        // Advance to current active action
        while (m_fields->m_currentActionIdx < m_fields->m_actions.size() &&
               m_fields->m_actions[m_fields->m_currentActionIdx].time < adjustedTime - 0.1) {
            m_fields->m_currentActionIdx++;
        }

        if (m_fields->m_currentActionIdx >= m_fields->m_actions.size()) return;

        const auto& nextAction = m_fields->m_actions[m_fields->m_currentActionIdx];
        double timeUntilClick = nextAction.time - adjustedTime;

        if (timeUntilClick > 0.0 && timeUntilClick <= lookahead) {
            float progress = static_cast<float>(1.0 - (timeUntilClick / lookahead));

            auto playerPos = m_player1->getPosition();
            auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

            std::string styleStr = Mod::get()->getSettingValue<std::string>("indicator-mode");
            int styleInt = static_cast<int>(parseIndicatorStyle(styleStr));
            std::string soundPackStr = Mod::get()->getSettingValue<std::string>("sound-pack");
            SoundPack soundPack = parseSoundPack(soundPackStr);

            float volume = static_cast<float>(Mod::get()->getSettingValue<double>("volume"));
            float contrast = static_cast<float>(Mod::get()->getSettingValue<double>("contrast"));

            bool isTargetHit = (timeUntilClick < 0.02);

            drawCue(
                m_fields->m_drawNode,
                styleInt,
                playerPos,
                1.0f,
                progress,
                0.0f,
                isTargetHit,
                static_cast<float>(Mod::get()->getSettingValue<double>("hw-x")),
                static_cast<float>(Mod::get()->getSettingValue<double>("hw-y")),
                static_cast<float>(Mod::get()->getSettingValue<double>("hw-scale")),
                static_cast<float>(Mod::get()->getSettingValue<double>("hw-opacity")),
                contrast
            );

            // Audio trigger on frame match
            if (isTargetHit && m_fields->m_lastJudgedFrame != nextAction.frame) {
                m_fields->m_lastJudgedFrame = nextAction.frame;
                if (nextAction.push) {
                    if (Mod::get()->getSettingValue<bool>("ticks")) {
                        AudioEngine::get()->playPressSound(soundPack, 1.0f, volume);
                    }
                } else {
                    if (Mod::get()->getSettingValue<bool>("release-sound")) {
                        AudioEngine::get()->playReleaseSound(soundPack, 0.9f, volume);
                    }
                }
            }
        }
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        m_fields->m_currentActionIdx = 0;
        m_fields->m_levelTime = 0.0;
        m_fields->m_lastJudgedFrame = -1;
    }

    void resetLevelFromStart() {
        PlayLayer::resetLevelFromStart();
        m_fields->m_currentActionIdx = 0;
        m_fields->m_levelTime = 0.0;
        m_fields->m_lastJudgedFrame = -1;
    }
};