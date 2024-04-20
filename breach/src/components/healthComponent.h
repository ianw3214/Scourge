#pragma once

#include "shade/game/entity/component/component.h"

class HealthComponent : public Shade::Component
{
public:
    HealthComponent(float maxHealth);

    float GetCurrentHealth() const { return mCurrHealth; }
    float GetMaxHealth() const { return mMaxHealth; }

    float DecrementHealth(float amount);

    // TODO: Consider making this a "flag component" so other components can access without depending on health component
    bool mIsInvulnerable = false;
private:
    float mCurrHealth = 0.f;
    float mMaxHealth = 0.f;
};