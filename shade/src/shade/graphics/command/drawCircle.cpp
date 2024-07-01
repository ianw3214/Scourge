#include "drawCircle.h"

#include "shade/graphics/renderer.h"
#include "shade/graphics/camera/camera.h"

// ======================================
Shade::DrawCircleCommand::DrawCircleCommand()
{

}

// ======================================
Shade::DrawCircleCommand::DrawCircleCommand(Vec2 point)
    : mPoint(point)
{

}

// ======================================
Shade::DrawCircleCommand::DrawCircleCommand(Vec2 point, float radius)
    : mPoint(point)
    , mRadius(radius)
{

}

// ======================================
Shade::DrawCircleCommand::DrawCircleCommand(Vec2 point, float radius, Colour colour)
    : mPoint(point)
    , mRadius(radius)
    , mColour(colour)
{

}

// ======================================
Shade::DrawCircleCommand::DrawCircleCommand(Vec2 point, float radius, Colour colour, bool normalized)
    : mPoint(point)
    , mRadius(radius)
    , mColour(colour)
    , mNormalized(normalized)
{

}

// ======================================
Shade::DrawCircleCommand::DrawCircleCommand(float x, float y)
    : mPoint(x, y)
{

}

// ======================================
Shade::DrawCircleCommand::DrawCircleCommand(float x, float y, float radius)
    : mPoint(x, y)
    , mRadius(radius)
{

}

// ======================================
Shade::DrawCircleCommand::DrawCircleCommand(float x, float y, float radius, Colour colour)
    : mPoint(x, y)
    , mRadius(radius)
    , mColour(colour)
{

}

// ======================================
Shade::DrawCircleCommand::DrawCircleCommand(float x, float y, float radius, Colour colour, bool normalized)
    : mPoint(x, y)
    , mRadius(radius)
    , mColour(colour)
    , mNormalized(normalized)
{

}

// ======================================
void Shade::DrawCircleCommand::Execute(RendererBase* renderer, const CameraInfo& camera) 
{
    const float x = mPoint.x + camera.mOffsetX;
    const float y = mPoint.y + camera.mOffsetY;
    if (mNormalized)
    {
        renderer->DrawCircleNormalized(x, y, mRadius, mColour);
    }
    else
    {
        renderer->DrawCircle(x, y, mRadius, mColour);
    }
}