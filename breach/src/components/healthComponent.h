#pragma once

#include "shade/game/entity/component/component.h"

class HealthComponent : public Shade::Component
{
public:
    HealthComponent(float maxHealth);

    float getCurrentHealth() const { return mCurrHealth; }
    float getMaxHealth() const { return mMaxHealth; }
private:
    float mCurrHealth = 0.f;
    float mMaxHealth = 0.f;
};