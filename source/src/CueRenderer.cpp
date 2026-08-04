#include "CueRenderer.hpp"
#include <cmath>

using namespace cocos2d;

void drawCue(
    CCDrawNode* drawNode,
    int styleInt,
    CCPoint pos,
    float scale,
    float progress,
    float holdLength,
    bool isTargetHit,
    float hwX,
    float hwY,
    float hwScale,
    float hwOpacity,
    float contrast
) {
    if (!drawNode) return;

    IndicatorStyle style = static_cast<IndicatorStyle>(styleInt);

    // Color definitions
    ccColor4F color = isTargetHit 
        ? ccc4f(0.0f, 1.0f, 0.2f, 1.0f) 
        : ccc4f(0.2f, 0.8f, 1.0f, 0.9f);

    ccColor4F darkHalo = ccc4f(0.0f, 0.0f, 0.0f, 0.4f * contrast);

    switch (style) {
        case IndicatorStyle::Ring: {
            // Ring collapses onto icon position
            float radius = (1.0f - progress) * 60.0f * scale + 15.0f * scale;
            if (contrast > 0.0f) {
                drawNode->drawCircle(pos, radius + 2.0f, darkHalo, 1.5f, darkHalo, 32);
            }
            drawNode->drawCircle(pos, radius, color, 2.0f * scale, color, 32);
            break;
        }

        case IndicatorStyle::Classic: {
            // Vertical target line and scrolling window bar
            float startY = pos.y - 40.0f * scale;
            float endY = pos.y + 40.0f * scale;
            float lineX = pos.x + (1.0f - progress) * 120.0f * scale;

            if (contrast > 0.0f) {
                drawNode->drawSegment(ccp(lineX - 1.0f, startY), ccp(lineX - 1.0f, endY), 3.0f * scale, darkHalo);
            }
            drawNode->drawSegment(ccp(lineX, startY), ccp(lineX, endY), 2.0f * scale, color);

            // Target stationary line
            drawNode->drawSegment(ccp(pos.x, startY - 10.0f), ccp(pos.x, endY + 10.0f), 1.5f * scale, ccc4f(1.0f, 1.0f, 1.0f, 0.6f));
            break;
        }

        case IndicatorStyle::Converge: {
            // Two horizontal lines meeting at center
            float offset = (1.0f - progress) * 80.0f * scale;
            float leftX = pos.x - offset;
            float rightX = pos.x + offset;

            drawNode->drawSegment(ccp(leftX - 20.0f, pos.y), ccp(leftX, pos.y), 2.5f * scale, color);
            drawNode->drawSegment(ccp(rightX, pos.y), ccp(rightX + 20.0f, pos.y), 2.5f * scale, color);
            break;
        }

        case IndicatorStyle::Pulse: {
            // Glowing reticle pulse
            float size = 20.0f * scale * (0.8f + progress * 0.4f);
            drawNode->drawDot(pos, size, color);
            break;
        }

        case IndicatorStyle::Highway: {
            // Guitar Hero style lane in screen corner
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            float laneX = winSize.width * hwX;
            float laneY = winSize.height * hwY;
            float laneWidth = 40.0f * hwScale;
            float laneHeight = 160.0f * hwScale;

            ccColor4F laneColor = ccc4f(0.1f, 0.1f, 0.1f, 0.7f * hwOpacity);
            ccColor4F noteColor = color;
            noteColor.a *= hwOpacity;

            // Draw background lane
            drawNode->drawSegment(ccp(laneX - laneWidth / 2, laneY), ccp(laneX - laneWidth / 2, laneY + laneHeight), 2.0f, laneColor);
            drawNode->drawSegment(ccp(laneX + laneWidth / 2, laneY), ccp(laneX + laneWidth / 2, laneY + laneHeight), 2.0f, laneColor);

            // Hit bar at bottom
            drawNode->drawSegment(ccp(laneX - laneWidth / 2 - 5, laneY + 20.0f), ccp(laneX + laneWidth / 2 + 5, laneY + 20.0f), 3.0f, ccc4f(1.0f, 1.0f, 1.0f, hwOpacity));

            // Falling note
            float noteY = laneY + 20.0f + progress * (laneHeight - 20.0f);
            drawNode->drawDot(ccp(laneX, noteY), 8.0f * hwScale, noteColor);

            // Hold line on highway if applicable
            if (holdLength > 0.0f) {
                float holdEndY = noteY + holdLength * 100.0f * hwScale;
                drawNode->drawSegment(ccp(laneX, noteY), ccp(laneX, holdEndY), 5.0f * hwScale, ccc4f(0.2f, 1.0f, 0.5f, 0.6f * hwOpacity));
            }
            break;
        }
    }
}