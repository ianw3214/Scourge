#include "healthComponent.h"

#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/entity/entity.h"

#include "components/combat/deathHandlingComponent.h"

#ifdef BUILD_BREACH_EDITOR
#include <imgui/imgui.h>
// ======================================
void HealthComponent::ShowImguiDetails() 
{
    ImGui::DragFloat("Max health", &mMaxHealth);
}
#endif

// ======================================
HealthComponent* HealthComponent::LoadFromFileHandle(Shade::KeyValueHandle handle)
{
    float maxHealth = HealthComponentDefaults::maxHealth;

    while (handle.IsValid())
    {
        if (handle.GetKey() == "max_health")
        {
            maxHealth = handle.TryGetFloat(maxHealth);
        }
        handle.ToNext();
    }

    return new HealthComponent(maxHealth);
}

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
    if (IsDead())
    {
        return mCurrHealth;
    }

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
        if (DeathHandlingComponent* deathHandler = mEntityRef->GetComponent<DeathHandlingComponent>())
        {
            // TODO: Remove hard-coded number here
            deathHandler->HandleDeath(3.f);
        }
        else
        {
            mEntityRef->MarkForDelete();
        }
    }

    return mCurrHealth;
}