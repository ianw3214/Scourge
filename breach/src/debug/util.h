#pragma once
#ifdef DEBUG_BREACH

#include "debug/debugModule.h"

namespace DebugUtils {

    static void DrawDebugLine(Shade::Vec2 position, float height)
    {
        CustomDebugModule::mDebugLines.emplace_back(CustomDebugModule::DebugLine{ position, height });
    }

}

#endif