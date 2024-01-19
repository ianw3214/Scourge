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
Shade::SpriteComponent::SpriteComponent(Entity& owner, float renderWidth, float renderHeight, std::string texturePath)
    : Component(owner)
    , mrenderWidth(renderWidth)
    , mrenderHeight(renderHeight)
    , mTextureHandle(ResourceHandle::Invalid)
{
    ResourceManager* resourceManager = ServiceProvider::GetCurrentProvider()->GetService<ResourceManager>();
    mTextureHandle = resourceManager->LoadResource<Texture>(texturePath);
}

// ======================================
float Shade::SpriteComponent::GetrenderWidth() const
{
    return mrenderWidth;
}

// ======================================
float Shade::SpriteComponent::GetrenderHeight() const
{
    return mrenderHeight;
}

// ======================================
std::unique_ptr<Shade::DrawTextureCommand> Shade::SpriteComponent::CreateRenderCommand()
{
    const float drawX = mEntityRef.GetPositionX();
    const float drawY = mEntityRef.GetPositionY();
    return std::make_unique<DrawTextureCommand>(drawX, drawY, mrenderWidth, mrenderHeight, mTextureHandle);
}