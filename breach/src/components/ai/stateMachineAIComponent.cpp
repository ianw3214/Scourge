#include "stateMachineAIComponent.h"

#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

// ======================================
StateMachineAIComponent::StateMachineAIComponent(const std::string& initialState, std::unordered_map<std::string, AIState> states)
    : mCurrentState(initialState)
    , mStates(states)
{

}

void StateMachineAIComponent::Update(float deltaSeconds) 
{
    if (mStates.find(mCurrentState) == mStates.end())
    {
        Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        logService->LogError("Current AI state does not exist: " + mCurrentState);
        return;
    }

    mStates[mCurrentState].mUpdate(deltaSeconds);

    for (const auto& transition : mStates[mCurrentState].mTransitions)
    {
        std::string nextState = transition(mEntityRef);
        if (!nextState.empty())
        {
            // TODO: Handle any logic that needs to run at transition time
            // TODO: Check that the next state actually exists
            mCurrentState = nextState;
        }
    }
}

// ======================================
void StateMachineAIComponent::SetEnabled(bool enabled)
{
    mEnabled = enabled;
}