#include "spriteComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/graphics/command/drawTexture.h"
#include "shade/instance/service/provider.h"
#include "shade/resource/manager.h"
#include "shade/graphics/texture.h"

// ======================================
Shade::SpriteComponent::SpriteComponent(Entity& owner)
    : Component(owner)
{

}

// ======================================
Shade::SpriteComponent::SpriteComponent(Entity& owner, float renderWidth, float renderHeight, std::string texturePath, int renderLayer)
    : Component(owner)
    , mRenderWidth(renderWidth)
    , mRenderHeight(renderHeight)
    , mRenderLayer(renderLayer)
    , mTextureHandle(ResourceHandle::Invalid)
{
    ResourceManager* resourceManager = ServiceProvider::GetCurrentProvider()->GetService<ResourceManager>();
    mTextureHandle = resourceManager->LoadResource<Texture>(texturePath);
}

// ======================================
float Shade::SpriteComponent::GetrenderWidth() const
{
    return mRenderWidth;
}

// ======================================
float Shade::SpriteComponent::GetrenderHeight() const
{
    return mRenderHeight;
}

// ======================================
std::unique_ptr<Shade::DrawTextureCommand> Shade::SpriteComponent::CreateRenderCommand()
{
    const float drawX = mEntityRef.GetPositionX();
    const float drawY = mEntityRef.GetPositionY();
    return std::make_unique<DrawTextureCommand>(drawX, drawY, mRenderWidth, mRenderHeight, mTextureHandle, mRenderLayer);
}