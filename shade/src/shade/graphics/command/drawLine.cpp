#include "drawLine.h"

#include "shade/graphics/renderer.h"

// ======================================
Shade::DrawLineCommand::DrawLineCommand()
{

}

// ======================================
Shade::DrawLineCommand::DrawLineCommand(Vec2 Point1, Vec2 Point2)
    : mPoint1(Point1)
    , mPoint2(Point2)
{

}

// ======================================
Shade::DrawLineCommand::DrawLineCommand(Vec2 Point1, Vec2 Point2, Colour LineColour)
    : mPoint1(Point1)
    , mPoint2(Point2)
    , mColour(LineColour)
{

}

// ======================================
Shade::DrawLineCommand::DrawLineCommand(Vec2 Point1, Vec2 Point2, Colour LineColour, bool Normalized)
    : mPoint1(Point1)
    , mPoint2(Point2)
    , mColour(LineColour)
    , mNormalized(Normalized)
{

}

// ======================================
Shade::DrawLineCommand::DrawLineCommand(float Point1x, float Point1y, float Point2x, float Point2y)
    : mPoint1(Point1x, Point1y)
    , mPoint2(Point2x, Point2y)
{

}

// ======================================
Shade::DrawLineCommand::DrawLineCommand(float Point1x, float Point1y, float Point2x, float Point2y, Colour LineColour)
    : mPoint1(Point1x, Point1y)
    , mPoint2(Point2x, Point2y)
    , mColour(LineColour)
{

}

// ======================================
Shade::DrawLineCommand::DrawLineCommand(float Point1x, float Point1y, float Point2x, float Point2y, Colour LineColour, bool Normalized)
    : mPoint1(Point1x, Point1y)
    , mPoint2(Point2x, Point2y)
    , mColour(LineColour)
    , mNormalized(Normalized)
{

}

// ======================================
void Shade::DrawLineCommand::Execute(RendererBase* Renderer) 
{
    if (mNormalized)
    {
        Renderer->DrawLineNormalized(mPoint1, mPoint2, mColour);
    }
    else
    {
        Renderer->DrawLine(mPoint1, mPoint2, mColour);
    }
}