#include "drawLine.h"

#include "shade/graphics/renderer.h"

// ======================================
Shade::DrawLineCommand::DrawLineCommand()
{

}

// ======================================
Shade::DrawLineCommand::DrawLineCommand(Vec2 point1, Vec2 point2)
    : mPoint1(point1)
    , mPoint2(point2)
{

}

// ======================================
Shade::DrawLineCommand::DrawLineCommand(Vec2 point1, Vec2 point2, Colour colour)
    : mPoint1(point1)
    , mPoint2(point2)
    , mColour(colour)
{

}

// ======================================
Shade::DrawLineCommand::DrawLineCommand(Vec2 point1, Vec2 point2, Colour colour, bool normalized)
    : mPoint1(point1)
    , mPoint2(point2)
    , mColour(colour)
    , mNormalized(normalized)
{

}

// ======================================
Shade::DrawLineCommand::DrawLineCommand(float point1x, float point1y, float point2x, float point2y)
    : mPoint1(point1x, point1y)
    , mPoint2(point2x, point2y)
{

}

// ======================================
Shade::DrawLineCommand::DrawLineCommand(float point1x, float point1y, float point2x, float point2y, Colour colour)
    : mPoint1(point1x, point1y)
    , mPoint2(point2x, point2y)
    , mColour(colour)
{

}

// ======================================
Shade::DrawLineCommand::DrawLineCommand(float point1x, float point1y, float point2x, float point2y, Colour colour, bool normalized)
    : mPoint1(point1x, point1y)
    , mPoint2(point2x, point2y)
    , mColour(colour)
    , mNormalized(normalized)
{

}

// ======================================
void Shade::DrawLineCommand::Execute(RendererBase* renderer) 
{
    if (mNormalized)
    {
        renderer->DrawLineNormalized(mPoint1, mPoint2, mColour);
    }
    else
    {
        renderer->DrawLine(mPoint1, mPoint2, mColour);
    }
}