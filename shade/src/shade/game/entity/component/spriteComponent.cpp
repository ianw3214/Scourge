#include "spriteComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/graphics/command/drawTexture.h"
#include "shade/instance/service/provider.h"
#include "shade/resource/manager.h"
#include "shade/graphics/texture.h"

// ======================================
Shade::SpriteComponent::SpriteComponent(float renderWidth, float renderHeight, std::string texturePath, int renderLayer, RenderAnchor renderAnchor)
    : mRenderWidth(renderWidth)
    , mRenderHeight(renderHeight)
    , mRenderAnchor(renderAnchor)
    , mRenderLayer(renderLayer)
    , mTextureHandle(ResourceHandle::Invalid)
{
    ResourceManager* resourceManager = ServiceProvider::GetCurrentProvider()->GetService<ResourceManager>();
    mTextureHandle = resourceManager->LoadResource<Texture>(texturePath);
}

// ======================================
float Shade::SpriteComponent::GetRenderWidth() const
{
    return mRenderWidth;
}

// ======================================
float Shade::SpriteComponent::GetRenderHeight() const
{
    return mRenderHeight;
}

// ======================================
float Shade::SpriteComponent::GetRenderX() const
{
    switch(mRenderAnchor) {
        case RenderAnchor::BOTTOM_LEFT: {
            return mEntityRef->GetPositionX();
        } break;
        case RenderAnchor::BOTTOM_MIDDLE:
        case RenderAnchor::MIDDLE: {
            return mEntityRef->GetPositionX() - mRenderWidth / 2.0f;
        } break;
        default: {
            return mEntityRef->GetPositionX();
        } break;
    }
}

// ======================================
float Shade::SpriteComponent::GetRenderY() const
{
    switch(mRenderAnchor) {
        case RenderAnchor::BOTTOM_LEFT: 
        case RenderAnchor::BOTTOM_MIDDLE: {
            return mEntityRef->GetPositionY();
        } break;
        case RenderAnchor::MIDDLE: {
            return mEntityRef->GetPositionY() - mRenderHeight / 2.0f;
        } break;
        default: {
            return mEntityRef->GetPositionX();
        } break;
    }
}


// ======================================
std::unique_ptr<Shade::DrawTextureCommand> Shade::SpriteComponent::CreateRenderCommand()
{
    const float drawX = GetRenderX();
    const float drawY = GetRenderY();
    return std::make_unique<DrawTextureCommand>(drawX, drawY, mRenderWidth, mRenderHeight, mTextureHandle, mRenderLayer);
}