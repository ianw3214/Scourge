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
Shade::DrawRectangleCommand::DrawRectangleCommand(const Shade::Box& box)
    : mPosition(box.mPosition)
    , mWidth(box.mWidth)
    , mHeight(box.mHeight)
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(const Shade::Box& box, Colour colour)
    : mPosition(box.mPosition)
    , mWidth(box.mWidth)
    , mHeight(box.mHeight)
    , mColour(colour)
{

}


// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(const Shade::Box& box, Colour colour, bool filled)
    : mPosition(box.mPosition)
    , mWidth(box.mWidth)
    , mHeight(box.mHeight)
    , mColour(colour)
    , mFilled(filled)
{

}

// ======================================
Shade::DrawRectangleCommand::DrawRectangleCommand(const Shade::Box& box, Colour colour, bool filled, bool normalized)
    : mPosition(box.mPosition)
    , mWidth(box.mWidth)
    , mHeight(box.mHeight)
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
    Shade::Vec2 adjustPosition = Shade::Vec2{ x, y };
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
            // TODO: Could this be optimized to not create a NEW vector every time it's called
            std::vector<Shade::Vec2> points = {
                adjustPosition,
                adjustPosition.Up(mHeight),
                adjustPosition.Up(mHeight).Right(mWidth),
                adjustPosition.Right(mWidth),
                adjustPosition
            };
            renderer->DrawLines(points, mColour);
        }
    }
}