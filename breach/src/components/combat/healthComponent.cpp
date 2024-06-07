#include "healthComponent.h"

#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/entity/entity.h"

// ======================================
HealthComponent::HealthComponent(float maxHealth)
    : mCurrHealth(maxHealth)
    , mMaxHealth(maxHealth)
{

}

// ======================================
void HealthComponent::Update(float deltaSeconds) 
{
    if (mDamageFlashTimer > 0.f)
    {
        Shade::SpriteComponent* sprite = mEntityRef->GetCachedSpriteComponent();
        const float colourScale = 1.f + mDamageFlashTimer * 3.f;
        const float colourScalePow = colourScale * colourScale * colourScale;
        sprite->SetColourMultiplier(Shade::Colour{ colourScalePow, colourScale, colourScale });
        mDamageFlashTimer -= deltaSeconds;
        if (mDamageFlashTimer <= 0.f)
        {
            sprite->ResetColourMultiplier();
        }
    }
}

// ======================================
float HealthComponent::DecrementHealth(float amount)
{
    if (mIsInvulnerable)
    {
        return mCurrHealth;
    }

    mDamageFlashTimer = 0.3f;

    // Decrement amount from current health
    mCurrHealth = mCurrHealth > amount ? mCurrHealth - amount : 0.f;

    // Death handling
    if (mCurrHealth <= 0.f)
    {
        mEntityRef->MarkForDelete();
    }

    return mCurrHealth;
}