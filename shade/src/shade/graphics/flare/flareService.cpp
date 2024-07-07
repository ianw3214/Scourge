#include "flareService.h"

#include "shade/graphics/command/drawTexture.h"

// ======================================
Shade::FlareService::FlareService()
    : Service("FlareService")
{

}

// ======================================
Shade::FlareService::~FlareService()
{

}

// ======================================
void Shade::FlareService::RenderVFX(std::vector<std::unique_ptr<RenderCommand>>& commandQueue, float deltaSeconds)
{
    for (Effect& effect : mEffects)
    {
        effect.mLifetime -= deltaSeconds;
        if (effect.mLifetime > 0.f)
        {
            if (effect.mTextureHandle.IsValid())
            {
                commandQueue.emplace_back(std::make_unique<DrawTextureCommand>(effect.mPosition, effect.mWidth, effect.mHeight, effect.mTextureHandle, effect.mRenderLayer));
            }
        }
    }

    mEffects.resize(std::distance(mEffects.begin(), std::remove_if(mEffects.begin(), mEffects.end(), [](const Effect& effect){
        return effect.mLifetime <= 0.f;
    })));
}

// ======================================
void Shade::FlareService::SpawnEffect(Effect effect)
{
    mEffects.push_back(effect);
}