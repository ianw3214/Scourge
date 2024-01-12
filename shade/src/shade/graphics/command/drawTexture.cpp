#include "drawTexture.h"

#include "shade/graphics/renderer.h"

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand()
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(Vec2 Position, float Width, float Height)
    : mPosition(Position)
    , mWidth(Width)
    , mHeight(Height)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(Vec2 Position, float Width, float Height, ResourceHandle TextureResource)
    : mPosition(Position)
    , mWidth(Width)
    , mHeight(Height)
    , mResourceHandle(TextureResource)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(Vec2 Position, float Width, float Height, ResourceHandle TextureResource, bool Normalized)
    : mPosition(Position)
    , mWidth(Width)
    , mHeight(Height)
    , mResourceHandle(TextureResource)
    , mNormalized(Normalized)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(Vec2 Position, float Width, float Height, ResourceHandle TextureResource, bool Normalized, TextureSourceInfo TextureSource)
    : mPosition(Position)
    , mWidth(Width)
    , mHeight(Height)
    , mResourceHandle(TextureResource)
    , mNormalized(Normalized)
    , mTextureSource(TextureSource)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(float xPosition, float yPosition, float Width, float Height)
    : mPosition(xPosition, yPosition)
    , mWidth(Width)
    , mHeight(Height)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(float xPosition, float yPosition, float Width, float Height, ResourceHandle TextureResource)
    : mPosition(xPosition, yPosition)
    , mWidth(Width)
    , mHeight(Height)
    , mResourceHandle(TextureResource)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(float xPosition, float yPosition, float Width, float Height, ResourceHandle TextureResource, bool Normalized)
    : mPosition(xPosition, yPosition)
    , mWidth(Width)
    , mHeight(Height)
    , mResourceHandle(TextureResource)
    , mNormalized(Normalized)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(float xPosition, float yPosition, float Width, float Height, ResourceHandle TextureResource, bool Normalized, TextureSourceInfo TextureSource)
    : mPosition(xPosition, yPosition)
    , mWidth(Width)
    , mHeight(Height)
    , mResourceHandle(TextureResource)
    , mNormalized(Normalized)
    , mTextureSource(TextureSource)
{

}

// ======================================
void Shade::DrawTextureCommand::Execute(RendererBase* Renderer) 
{
    if (mNormalized)
    {
        Renderer->DrawTextureNormalized(mPosition, mWidth, mHeight, mResourceHandle, mTextureSource);
    }
    else {
        Renderer->DrawTexture(mPosition, mWidth, mHeight, mResourceHandle, mTextureSource);
    }
}