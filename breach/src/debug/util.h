#pragma once
#ifdef DEBUG_BREACH

#include "debug/debugModule.h"

namespace DebugUtils {

    static void DrawDebugLine(Shade::Vec2 position1, Shade::Vec2 position2)
    {
        CustomDebugModule::mDebugLines.emplace_back(CustomDebugModule::DebugLine{ position1, position2 });
    }

    static void DrawDebugRect(Shade::Vec2 position, float width, float height, Shade::Colour colour)
    {
        CustomDebugModule::mDebugRects.emplace_back(CustomDebugModule::DebugRect{ position, width, height, colour });
    }

}

#endif