#include "drawTexture.h"

#include "shade/graphics/renderer.h"

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand()
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(Vec2 position, float width, float height)
    : mPosition(position)
    , mWidth(width)
    , mHeight(height)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(Vec2 position, float width, float height, ResourceHandle textureResource)
    : mPosition(position)
    , mWidth(width)
    , mHeight(height)
    , mResourceHandle(textureResource)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(Vec2 position, float width, float height, ResourceHandle textureResource, bool normalized)
    : mPosition(position)
    , mWidth(width)
    , mHeight(height)
    , mResourceHandle(textureResource)
    , mNormalized(normalized)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(Vec2 position, float width, float height, ResourceHandle textureResource, bool normalized, textureSourceInfo textureSource)
    : mPosition(position)
    , mWidth(width)
    , mHeight(height)
    , mResourceHandle(textureResource)
    , mNormalized(normalized)
    , mTextureSource(textureSource)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(float xPosition, float yPosition, float width, float height)
    : mPosition(xPosition, yPosition)
    , mWidth(width)
    , mHeight(height)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(float xPosition, float yPosition, float width, float height, ResourceHandle textureResource)
    : mPosition(xPosition, yPosition)
    , mWidth(width)
    , mHeight(height)
    , mResourceHandle(textureResource)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(float xPosition, float yPosition, float width, float height, ResourceHandle textureResource, bool normalized)
    : mPosition(xPosition, yPosition)
    , mWidth(width)
    , mHeight(height)
    , mResourceHandle(textureResource)
    , mNormalized(normalized)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(float xPosition, float yPosition, float width, float height, ResourceHandle textureResource, bool normalized, textureSourceInfo textureSource)
    : mPosition(xPosition, yPosition)
    , mWidth(width)
    , mHeight(height)
    , mResourceHandle(textureResource)
    , mNormalized(normalized)
    , mTextureSource(textureSource)
{

}

// ======================================
void Shade::DrawTextureCommand::Execute(RendererBase* renderer) 
{
    if (mNormalized)
    {
        renderer->DrawTextureNormalized(mPosition, mWidth, mHeight, mResourceHandle, mTextureSource);
    }
    else {
        renderer->DrawTexture(mPosition, mWidth, mHeight, mResourceHandle, mTextureSource);
    }
}