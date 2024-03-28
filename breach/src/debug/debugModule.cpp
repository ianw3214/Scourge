#ifdef DEBUG_BREACH
#include "debugModule.h"

#include "shade/graphics/command/drawLine.h"
#include "shade/graphics/command/drawRectangle.h"

// ======================================
// Don't bother storing this properly since it's just debug info
std::vector<CustomDebugModule::DebugLine> CustomDebugModule::mDebugLines;
std::vector<CustomDebugModule::DebugRect> CustomDebugModule::mDebugRects;

// ======================================
void CustomDebugModule::Render(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue)
{
    for (const auto& rect : mDebugRects)
    {
        commandQueue.emplace_back(std::make_unique<Shade::DrawRectangleCommand>(rect.mPos.x, rect.mPos.y, rect.mWidth, rect.mHeight, rect.mColour));
    }
    for (const auto& line : mDebugLines)
    {
        commandQueue.emplace_back(std::make_unique<Shade::DrawLineCommand>(line.mPos.x, line.mPos.y, line.mPos.x, line.mPos.y + line.mHeight, Shade::Colour{ 1.0, 1.0, 1.0 }));
    }
    mDebugRects.clear();
    mDebugLines.clear();
}

#endif