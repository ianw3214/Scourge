#include "blackboardComponent.h"

#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

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
        Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        logService->LogError("Blackboard field does not exist: '" + key + '\'');
        return 0.f;
    }
    return it->second;
}