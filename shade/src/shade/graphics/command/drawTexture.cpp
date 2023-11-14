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
Shade::DrawTextureCommand::DrawTextureCommand(Vec2 Position, float Width, float Height, const std::string& TexturePath)
    : mPosition(Position)
    , mWidth(Width)
    , mHeight(Height)
    , mTexturePath(TexturePath)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(Vec2 Position, float Width, float Height, const std::string& TexturePath, bool Normalized)
    : mPosition(Position)
    , mWidth(Width)
    , mHeight(Height)
    , mTexturePath(TexturePath)
    , mNormalized(Normalized)
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
Shade::DrawTextureCommand::DrawTextureCommand(float xPosition, float yPosition, float Width, float Height, const std::string& TexturePath)
    : mPosition(xPosition, yPosition)
    , mWidth(Width)
    , mHeight(Height)
    , mTexturePath(TexturePath)
{

}

// ======================================
Shade::DrawTextureCommand::DrawTextureCommand(float xPosition, float yPosition, float Width, float Height, const std::string& TexturePath, bool Normalized)
    : mPosition(xPosition, yPosition)
    , mWidth(Width)
    , mHeight(Height)
    , mTexturePath(TexturePath)
    , mNormalized(Normalized)
{

}

// ======================================
void Shade::DrawTextureCommand::Execute(RendererBase* Renderer) 
{
    if (mNormalized)
    {
        Renderer->DrawTextureNormalized(mPosition, mWidth, mHeight, mTexturePath);
    }
    else {
        Renderer->DrawTexture(mPosition, mWidth, mHeight, mTexturePath);
    }
}