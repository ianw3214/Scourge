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
Shade::DrawRectangleCommand::DrawRectangleCommand(Vec2 position, float width, float height, Colour colour, bool filled)
    : mPosition(position)
    , mWidth(width)
    , mHeight(height)
    , mColour(colour)
    , mFilled(filled)
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(Vec2 position, float width, float height, Colour colour, bool filled, bool normalized)
    : mPosition(position)
    , mWidth(width)
    , mHeight(height)
    , mColour(colour)
    , mFilled(filled)
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
Shade::DrawRectangleCommand::DrawRectangleCommand(float xPosition, float yPosition, float width, float height, Colour colour, bool filled)
    : mPosition(xPosition, yPosition)
    , mWidth(width)
    , mHeight(height)
    , mColour(colour)
    , mFilled(filled)
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(float xPosition, float yPosition, float width, float height, Colour colour, bool filled, bool normalized)
    : mPosition(xPosition, yPosition)
    , mWidth(width)
    , mHeight(height)
    , mColour(colour)
    , mFilled(filled)
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
        // TODO: Filled
        renderer->DrawRectangleNormalized(x, y, mWidth, mHeight, mColour);
    }
    else {
        if (mFilled)
        {
            renderer->DrawRectangle(x, y, mWidth, mHeight, mColour);   
        }
        else
        {
            // TODO: Implementing this
            //  - maybe optimize
            std::vector<Shade::Vec2> points = {
                mPosition,
                mPosition.Up(mHeight),
                mPosition.Up(mHeight).Right(mWidth),
                mPosition.Right(mWidth),
                mPosition
            };
            renderer->DrawLines(points, mColour);
        }
    }
}