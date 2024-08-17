#include "spriteComponent.h"

#include "shade/file/keyValueFile.h"
#include "shade/game/entity/entity.h"
#include "shade/graphics/command/drawTexture.h"
#include "shade/graphics/command/setColourMultiplier.h"
#include "shade/graphics/common.h"
#include "shade/instance/service/provider.h"
#include "shade/resource/manager.h"
#include "shade/graphics/texture.h"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

const std::string Shade::SpriteComponent::ComponentID = "sprite";

#ifdef BUILD_SHADE_EDITOR
// ======================================
void Shade::SpriteComponent::ShowImguiDetails() 
{
    const bool widthChanged = ImGui::DragFloat("width", &mRenderWidth, 1.0f, 0.f, 500.f, "%.1f");
    const bool heightChanged = ImGui::DragFloat("height", &mRenderHeight, 1.0f, 0.f, 500.f, "%.1f");
    // TODO: Implement render anchor selection
    ImGui::DragInt("render layer", &mRenderLayer, 1.0f, -10, 10);
    ImGui::Checkbox("constant depth", &mConstantDepth);
    const bool texturePathChanged = ImGui::InputText("texture path", &mTexturePath);

    if (widthChanged || heightChanged)
    {
        // TODO: Adjust camera to center sprite
    }
    if (texturePathChanged)
    {
        ResourceManager* resourceManager = ServiceProvider::GetCurrentProvider()->GetService<ResourceManager>();
        Shade::ResourceHandle newTextureHandle = resourceManager->LoadResource<Texture>(mTexturePath);
        if (newTextureHandle.IsValid())
        {
            mTextureHandle = newTextureHandle;
        }
        else
        {
            mTextureHandle = Shade::ResourceHandle::Invalid;
        }
    }
}

// ======================================
void Shade::SpriteComponent::SaveToKeyValueFile(Shade::KeyValueFile& file) const 
{
    file.AddStringEntry("path", mTexturePath);
    file.AddStringEntry("anchor", RenderUtil::RenderAnchorToString(mRenderAnchor));
}
#endif

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
#ifdef BUILD_SHADE_EDITOR
    mTexturePath = texturePath;
#endif
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

#ifdef BUILD_SHADE_EDITOR
    mTexturePath = texturePath;
#endif
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
    if (!mTextureHandle.IsValid())
    {
        return;
    }
    
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