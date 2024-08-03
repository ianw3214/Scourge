#pragma once

#include "shade/game/entity/component/component.h"

#include "shade/file/keyValueFile.h"

// ======================================
namespace HealthComponentDefaults {
    constexpr float maxHealth = 0.f;
}

// ======================================
class HealthComponent : public Shade::Component
{
public:
    static HealthComponent* LoadFromFileHandle(Shade::KeyValueHandle handle);
public:
    HealthComponent(float maxHealth);

    void Update(float deltaSeconds) override;

    // TODO: IsDead might want to be put into a common entity function somehow since it's so commonly used
    bool IsDead() const { return mCurrHealth <= 0.f; }
    float GetCurrentHealth() const { return mCurrHealth; }
    float GetMaxHealth() const { return mMaxHealth; }

    float DecrementHealth(float amount);

    // TODO: Consider making this a "flag component" so other components can access without depending on health component
    bool mIsInvulnerable = false;
private:
    float mCurrHealth = 0.f;
    float mDamageFlashTimer = 0.f;

    // Health component properties
    float mMaxHealth = HealthComponentDefaults::maxHealth;
};