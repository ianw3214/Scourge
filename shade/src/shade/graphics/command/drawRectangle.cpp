#include "drawRectangle.h"

#include "shade/graphics/renderer.h"

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand()
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(Vec2 Position, float Width, float Height)
    : mPosition(Position)
    , mWidth(Width)
    , mHeight(Height)
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(Vec2 Position, float Width, float Height, Colour RectColour)
    : mPosition(Position)
    , mWidth(Width)
    , mHeight(Height)
    , mColour(RectColour)
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(Vec2 Position, float Width, float Height, Colour RectColour, bool Normalized)
    : mPosition(Position)
    , mWidth(Width)
    , mHeight(Height)
    , mColour(RectColour)
    , mNormalized(Normalized)
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(float xPosition, float yPosition, float Width, float Height)
    : mPosition(xPosition, yPosition)
    , mWidth(Width)
    , mHeight(Height)
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(float xPosition, float yPosition, float Width, float Height, Colour RectColour)
    : mPosition(xPosition, yPosition)
    , mWidth(Width)
    , mHeight(Height)
    , mColour(RectColour)
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(float xPosition, float yPosition, float Width, float Height, Colour RectColour, bool Normalized)
    : mPosition(xPosition, yPosition)
    , mWidth(Width)
    , mHeight(Height)
    , mColour(RectColour)
    , mNormalized(Normalized)
{

}


// ======================================
void Shade::DrawRectangleCommand::Execute(RendererBase* Renderer) 
{
    if (mNormalized)
    {
        Renderer->DrawRectangleNormalized(mPosition, mWidth, mHeight, mColour);
    }
    else {
        Renderer->DrawRectangle(mPosition, mWidth, mHeight, mColour);
    }
}