#include "drawTexture.h"

#include "shade/graphics/renderer.h"
#include "shade/graphics/camera/camera.h"

// ======================================
// Layering constants
//  - Consider whether this should live here or be extended to other rendering mechanisms
//  - only z depths between [-1, 1] are rendered, so only layers between [-10, 10] will be rendered accordingly
namespace {
    constexpr float LAYER_CHUNK = 0.1f;
}

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
Shade::DrawTextureCommand::DrawTextureCommand(Vec2 position, float width, float height, ResourceHandle textureResource, int layer)
    : mPosition(position)
    , mWidth(width)
    , mHeight(height)
    , mResourceHandle(textureResource)
    , mLayer(layer)
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
Shade::DrawTextureCommand::DrawTextureCommand(float xPosition, float yPosition, float width, float height, ResourceHandle textureResource, int layer)
    : mPosition(xPosition, yPosition)
    , mWidth(width)
    , mHeight(height)
    , mResourceHandle(textureResource)
    , mLayer(layer)
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
Shade::DrawTextureCommand::DrawTextureCommand(float xPosition, float yPosition, float width, float height, ResourceHandle textureResource, bool normalized, textureSourceInfo textureSource, int layer)
    : mPosition(xPosition, yPosition)
    , mWidth(width)
    , mHeight(height)
    , mResourceHandle(textureResource)
    , mNormalized(normalized)
    , mTextureSource(textureSource)
    , mLayer(layer)
{

}

// ======================================
void Shade::DrawTextureCommand::Execute(RendererBase* renderer, const CameraInfo& camera) 
{
    const float x = mPosition.x + camera.mOffsetX;
    const float y = mPosition.y + camera.mOffsetY;
    // Calculate depth based on the layer
    //  - take y position into consideration here (The larger the y, the "deeper" the z)
    //  - very janky current implementation, need to consider:
    //      - What if textures want to "opt out" of this sorting
    //      - What if a texture's y position is greater than 1000.f?
    //      - What if a texture wants to offset from it's base y position?
    const float depth = -mLayer * LAYER_CHUNK + (y / 10000.f);
    if (mNormalized)
    {
        renderer->DrawTextureNormalized(x, y, mWidth, mHeight, mResourceHandle, mTextureSource, depth);
    }
    else {
        renderer->DrawTexture(x, y, mWidth, mHeight, mResourceHandle, mTextureSource, depth);
    }
}