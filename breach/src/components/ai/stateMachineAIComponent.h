#pragma once

#include "shade/game/entity/component/component.h"

#include "shade/file/keyValueFile.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

// forward declarations
namespace Shade {
    class Entity;
};

// Definition of AI behaviour for a specified state using function pointers
class AIState {
public:
    typedef std::function<void(Shade::Entity*, float)> UpdateFunc;
    typedef std::function<void(Shade::Entity*)> OnEnterFunc;
    typedef std::function<std::string(Shade::Entity*)> CheckTransitionFunc;

    UpdateFunc mUpdate;
    OnEnterFunc mOnEnter;
    std::vector<CheckTransitionFunc> mTransitions;
};

// A state machine based implementation of AI decision making
class StateMachineAIComponent : public Shade::Component {
public:
    static StateMachineAIComponent* LoadFromFileHandle(Shade::KeyValueHandle handle);
public:
    StateMachineAIComponent(const std::string& initialState, std::unordered_map<std::string, AIState> states);

    void Update(float deltaSeconds) override;

    void SetEnabled(bool enabled = true);

private:
    // Change state should never be directly called, the state machine itself should handle all changes
    void ChangeState(const std::string& newState);

private:
    std::string mCurrentState;
    std::unordered_map<std::string, AIState> mStates;
    bool mEnabled = true;
};