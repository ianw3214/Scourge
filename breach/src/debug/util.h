#pragma once
#ifdef DEBUG_BREACH

#include "debug/debugModule.h"

namespace DebugUtils {

    static void DrawDebugLine(Shade::Vec2 position, float height)
    {
        CustomDebugModule::mDebugLines.emplace_back(CustomDebugModule::DebugLine{ position, height });
    }

    static void DrawDebugRect(Shade::Vec2 position, float width, float height, Shade::Colour colour)
    {
        CustomDebugModule::mDebugRects.emplace_back(CustomDebugModule::DebugRect{ position, width, height, colour });
    }

}

#endif