#include "stateMachineAIComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

#include "components/combat/healthComponent.h"

// ======================================
StateMachineAIComponent::StateMachineAIComponent(const std::string& initialState, std::unordered_map<std::string, AIState> states)
    : mCurrentState(initialState)
    , mStates(states)
{

}

void StateMachineAIComponent::Update(float deltaSeconds) 
{
    HealthComponent* health = mEntityRef->GetComponent<HealthComponent>();
    if (health && health->IsDead())
    {
        return;
    }
    
    if (mStates.find(mCurrentState) == mStates.end())
    {
        Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        logService->LogError("Current AI state does not exist: " + mCurrentState);
        return;
    }

    if (mStates[mCurrentState].mUpdate)
    {
        mStates[mCurrentState].mUpdate(mEntityRef, deltaSeconds);
    }

    for (const auto& transition : mStates[mCurrentState].mTransitions)
    {
        std::string nextState = transition(mEntityRef);
        if (!nextState.empty())
        {
            ChangeState(nextState);
            break;
        }
    }
}

// ======================================
void StateMachineAIComponent::SetEnabled(bool enabled)
{
    mEnabled = enabled;
}

// ======================================
void StateMachineAIComponent::ChangeState(const std::string& newState)
{
    auto it = mStates.find(newState);
    if (it == mStates.end())
    {
        Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        logService->LogError("New AI state does not exist: " + newState);
    }
    mCurrentState = newState;
    if (it->second.mOnEnter)
    {
        it->second.mOnEnter(mEntityRef);
    }
}