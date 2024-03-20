#ifdef DEBUG_BREACH
#include "debugModule.h"

#include "shade/graphics/command/drawLine.h"

// ======================================
// Don't bother storing this properly since it's just debug info
std::vector<CustomDebugModule::DebugLine> CustomDebugModule::mDebugLines;

// ======================================
void CustomDebugModule::Render(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue)
{
    for (const auto& line : mDebugLines)
    {
        commandQueue.emplace_back(std::make_unique<Shade::DrawLineCommand>(line.mPos.x, line.mPos.y, line.mPos.x, line.mPos.y + line.mHeight, Shade::Colour{ 1.0, 1.0, 1.0 }));
    }
    mDebugLines.clear();
}

#endif