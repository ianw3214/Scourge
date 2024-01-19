#include "drawRectangle.h"

#include "shade/graphics/renderer.h"

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand()
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(Vec2 position, float width, float height)
    : mPosition(position)
    , mWidth(width)
    , mHeight(height)
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(Vec2 position, float width, float height, Colour colour)
    : mPosition(position)
    , mWidth(width)
    , mHeight(height)
    , mColour(colour)
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(Vec2 position, float width, float height, Colour colour, bool normalized)
    : mPosition(position)
    , mWidth(width)
    , mHeight(height)
    , mColour(colour)
    , mNormalized(normalized)
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(float xPosition, float yPosition, float width, float height)
    : mPosition(xPosition, yPosition)
    , mWidth(width)
    , mHeight(height)
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(float xPosition, float yPosition, float width, float height, Colour colour)
    : mPosition(xPosition, yPosition)
    , mWidth(width)
    , mHeight(height)
    , mColour(colour)
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(float xPosition, float yPosition, float width, float height, Colour colour, bool normalized)
    : mPosition(xPosition, yPosition)
    , mWidth(width)
    , mHeight(height)
    , mColour(colour)
    , mNormalized(normalized)
{

}


// ======================================
void Shade::DrawRectangleCommand::Execute(RendererBase* renderer) 
{
    if (mNormalized)
    {
        renderer->DrawRectangleNormalized(mPosition, mWidth, mHeight, mColour);
    }
    else {
        renderer->DrawRectangle(mPosition, mWidth, mHeight, mColour);
    }
}