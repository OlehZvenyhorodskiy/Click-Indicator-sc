#pragma once
#include "Macro.hpp"
#include <cocos2d.h>

void drawCue(
    cocos2d::CCDrawNode* drawNode,
    int styleInt,
    cocos2d::CCPoint pos,
    float scale,
    float progress,
    float holdLength,
    bool isTargetHit,
    float hwX,
    float hwY,
    float hwScale,
    float hwOpacity,
    float contrast
);