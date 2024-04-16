#include "blackboardComponent.h"

// ======================================
BlackboardComponent::BlackboardComponent() = default;

// ======================================
void BlackboardComponent::StoreFloat(const std::string& key, float value)
{
    mFloats[key] = value;
}

// ======================================
float BlackboardComponent::GetFloat(const std::string& key) const
{
    auto it = mFloats.find(key);
    if (it == mFloats.end())
    {
        // TODO: Log an error
        return 0.f;
    }
    return it->second;
}