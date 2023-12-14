#include "spriteComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/graphics/command/drawTexture.h"
#include "shade/instance/service/provider.h"
#include "shade/resource/manager.h"
#include "shade/graphics/texture.h"

// ======================================
Shade::SpriteComponent::SpriteComponent(Entity& Owner)
    : Component(Owner)
{

}

// ======================================
Shade::SpriteComponent::SpriteComponent(Entity& Owner, float RenderWidth, float RenderHeight, std::string TexturePath)
    : Component(Owner)
    , mRenderWidth(RenderWidth)
    , mRenderHeight(RenderHeight)
    , mTextureHandle(ResourceHandle::Invalid)
{
    ResourceManager* Manager = ServiceProvider::GetCurrentProvider()->GetService<ResourceManager>();
    mTextureHandle = Manager->LoadResource<Texture>(TexturePath);
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
std::unique_ptr<Shade::DrawTextureCommand> Shade::SpriteComponent::CreateRenderCommand()
{
    const float DrawX = mEntityRef.GetPositionX();
    const float DrawY = mEntityRef.GetPositionY();
    return std::make_unique<DrawTextureCommand>(DrawX, DrawY, mRenderWidth, mRenderHeight, mTextureHandle);
}