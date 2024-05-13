#ifdef DEBUG_BREACH
#include "debugModule.h"

#include "shade/instance/service/provider.h"
#include "shade/graphics/camera/camera.h"
#include "shade/graphics/command/drawLine.h"
#include "shade/graphics/command/drawRectangle.h"

#include "map/map.h"

// ======================================
// Don't bother storing this properly since it's just debug info
std::vector<CustomDebugModule::DebugLine> CustomDebugModule::mDebugLines;
std::vector<CustomDebugModule::DebugRect> CustomDebugModule::mDebugRects;

// ======================================
void CustomDebugModule::Update(float deltaSeconds)
{
    for (DebugRect& rect : mDebugRects)
    {
        if (rect.mDelay > 0.f)
        {
            rect.mDelay = std::max(rect.mDelay - deltaSeconds, 0.f);
        }
    }
    for (DebugLine& line : mDebugLines)
    {
        if (line.mDelay > 0.f)
        {
            line.mDelay = std::max(line.mDelay - deltaSeconds, 0.f);
        }
    }
}

// ======================================
void CustomDebugModule::Render(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue)
{
    Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();

    // Draw map layout
    MapService* map = Shade::ServiceProvider::GetCurrentProvider()->GetService<MapService>();
    const MapLayout& layout = map->GetLayout();
    for (const Shade::Box& playZone : layout.GetPlayZones())
    {
        const float x = playZone.mPosition.x + camera->GetCameraInfo().mOffsetX;
        const float y = playZone.mPosition.y + camera->GetCameraInfo().mOffsetY;
        commandQueue.emplace_back(std::make_unique<Shade::DrawRectangleCommand>(x, y, playZone.mWidth, playZone.mHeight, Shade::Colour{ 0.3f, 1.0f, 0.5f }, false));
    }

    // Other layouts
    for (const DebugRect& rect : mDebugRects)
    {
        commandQueue.emplace_back(std::make_unique<Shade::DrawRectangleCommand>(rect.mPos.x, rect.mPos.y, rect.mWidth, rect.mHeight, rect.mColour));
    }
    for (const DebugLine& line : mDebugLines)
    {
        commandQueue.emplace_back(std::make_unique<Shade::DrawLineCommand>(line.mPos1, line.mPos2, Shade::Colour{ 1.0, 1.0, 1.0 }));
    }
    mDebugRects.erase(std::remove_if(mDebugRects.begin(), mDebugRects.end(), [](const DebugRect& rect){ return rect.mDelay == 0.f; }), mDebugRects.end());
    mDebugLines.erase(std::remove_if(mDebugLines.begin(), mDebugLines.end(), [](const DebugLine& line){ return line.mDelay == 0.f; }), mDebugLines.end());
}

#endif