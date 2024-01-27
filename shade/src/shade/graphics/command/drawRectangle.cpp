#include "drawRectangle.h"

#include "shade/graphics/renderer.h"
#include "shade/graphics/camera/camera.h"

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
void Shade::DrawRectangleCommand::Execute(RendererBase* renderer, const CameraInfo& camera) 
{
    const float x = mPosition.x + camera.mOffsetX;
    const float y = mPosition.y + camera.mOffsetY;
    if (mNormalized)
    {
        renderer->DrawRectangleNormalized(x, y, mWidth, mHeight, mColour);
    }
    else {
        renderer->DrawRectangle(x, y, mWidth, mHeight, mColour);
    }
}