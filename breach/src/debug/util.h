#pragma once
#ifdef DEBUG_BREACH

#include "debug/debugModule.h"

namespace DebugUtils {

    static void DrawDebugLine(Shade::Vec2 position1, Shade::Vec2 position2, float delay = 0.f)
    {
        CustomDebugModule::mDebugLines.emplace_back(CustomDebugModule::DebugLine{ position1, position2, delay });
    }

    static void DrawDebugRect(Shade::Vec2 position, float width, float height, Shade::Colour colour, float delay = 0.f)
    {
        CustomDebugModule::mDebugRects.emplace_back(CustomDebugModule::DebugRect{ position, width, height, colour, delay });
    }

    static void DrawDebugRectOutline(Shade::Vec2 position, float width, float height, Shade::Colour colour, float delay = 0.f)
    {
        const Shade::Vec2 bottomLeft = position;
        const Shade::Vec2 topRight = bottomLeft.Right(width).Up(height);
        CustomDebugModule::mDebugLines.emplace_back(CustomDebugModule::DebugLine{ bottomLeft, bottomLeft.Right(width), delay });
        CustomDebugModule::mDebugLines.emplace_back(CustomDebugModule::DebugLine{ bottomLeft, bottomLeft.Up(height), delay });
        CustomDebugModule::mDebugLines.emplace_back(CustomDebugModule::DebugLine{ topRight.Left(width), topRight, delay });
        CustomDebugModule::mDebugLines.emplace_back(CustomDebugModule::DebugLine{ topRight.Down(height), topRight, delay });
    }

}

#endif