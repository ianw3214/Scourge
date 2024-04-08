#include "healthComponent.h"

// ======================================
HealthComponent::HealthComponent(float maxHealth)
    : mCurrHealth(maxHealth)
    , mMaxHealth(maxHealth)
{

}

// ======================================
float HealthComponent::DecrementHealth(float amount)
{
    mCurrHealth = mCurrHealth > amount ? mCurrHealth - amount : 0.f;
    return mCurrHealth;
}