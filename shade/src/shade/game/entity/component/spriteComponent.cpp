#include "spriteComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/graphics/command/drawTexture.h"
#include "shade/graphics/command/setColourMultiplier.h"
#include "shade/instance/service/provider.h"
#include "shade/resource/manager.h"
#include "shade/graphics/texture.h"

// ======================================
Shade::SpriteComponent::SpriteComponent(const std::string& texturePath, int renderLayer, RenderAnchor renderAnchor, bool constantDepth)
    : mRenderAnchor(renderAnchor)
    , mRenderLayer(renderLayer)
    , mConstantDepth(constantDepth)
    , mTextureHandle(ResourceHandle::Invalid)
{
    ResourceManager* resourceManager = ServiceProvider::GetCurrentProvider()->GetService<ResourceManager>();
    mTextureHandle = resourceManager->LoadResource<Texture>(texturePath);

    // TODO: Warning for sprite that failed to load?
    Texture* loadedTexture = resourceManager->GetResource<Texture>(mTextureHandle);
    if (loadedTexture != nullptr)
    {
        mRenderWidth = loadedTexture->GetWidth();
        mRenderHeight = loadedTexture->GetHeight();
    }
}

// ======================================
Shade::SpriteComponent::SpriteComponent(float renderWidth, float renderHeight, const std::string& texturePath, int renderLayer, RenderAnchor renderAnchor, bool constantDepth)
    : mRenderWidth(renderWidth)
    , mRenderHeight(renderHeight)
    , mRenderAnchor(renderAnchor)
    , mRenderLayer(renderLayer)
    , mConstantDepth(constantDepth)
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
    return RenderUtil::GetXForRenderAnchor(mEntityRef->GetPositionX(), mRenderWidth, mRenderAnchor);
}

// ======================================
float Shade::SpriteComponent::GetRenderY() const
{
    return RenderUtil::GetYForRenderAnchor(mEntityRef->GetPositionY(), mRenderHeight, mRenderAnchor);
}

// ======================================
void Shade::SpriteComponent::SetColourMultiplier(Colour colour)
{
    mColourMultiplier = colour;
}

// ======================================
void Shade::SpriteComponent::ResetColourMultiplier()
{
    mColourMultiplier = Colour{ 1.f, 1.f, 1.f };
}

// ======================================
void Shade::SpriteComponent::AddRenderCommands(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue)
{
    bool useMultiplier = false;
    if (mColourMultiplier.r != 1.f || mColourMultiplier.g != 1.f || mColourMultiplier.b != 1.f)
    {
        useMultiplier = true;
        commandQueue.emplace_back(std::make_unique<SetColourMultiplierCommand>(mColourMultiplier));
    }
    const float drawX = GetRenderX();
    const float drawY = GetRenderY();
    commandQueue.emplace_back(std::make_unique<DrawTextureCommand>(drawX, drawY, mRenderWidth, mRenderHeight, mTextureHandle, mRenderLayer, mConstantDepth));
    if (useMultiplier)
    {
        commandQueue.emplace_back(std::make_unique<ResetColourMultiplierCommand>());
    }
}