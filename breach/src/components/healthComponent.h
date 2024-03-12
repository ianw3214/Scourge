#pragma once

#include "shade/game/entity/component/component.h"

class HealthComponent : public Shade::Component
{
public:
    HealthComponent(float maxHealth);
private:
    float mCurrHealth;
    float mMaxHealth;
};