#include "stateMachineAIComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

#include "components/combat/attackComponent.h"
#include "components/combat/healthComponent.h"
#include "components/movement/locomotionComponent.h"
#include "components/movement/staggerComponent.h"
#include "playerRegistry.h"

const std::string StateMachineAIComponent::ComponentID = "ai";

// ======================================
StateMachineAIComponent* StateMachineAIComponent::LoadFromFileHandle(Shade::KeyValueHandle handle)
{
    // TODO: Hard-coded state machine AI, need to actually implement AI parsing in the future

    // AI state machine definition
    // TODO: Move temp cooldown code into blackboard or something similar
    static float attackCooldown = 0.f;
    static float specialCooldown = 0.f;
    AIState idleState, moveState, attackState, specialState;
    idleState.mUpdate = [](Shade::Entity* AIEntity, float deltaSeconds) {
        if (attackCooldown > 0.f)
        {
            attackCooldown -= deltaSeconds;
        }
        if (specialCooldown > 0.f)
        {
            specialCooldown -= deltaSeconds;
        }
    };
    idleState.mTransitions.push_back([](Shade::Entity* AIEntity){ 
        StaggerComponent* stagger = AIEntity->GetComponent<StaggerComponent>();
        if (stagger && stagger->IsStaggering())
        {
            return "";
        }

        Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
        const float diff_x = AIEntity->GetPositionX() - player->GetPositionX();
        const float diff_y = AIEntity->GetPositionY() - player->GetPositionY();
        if ((diff_x * diff_x + diff_y * diff_y) > 40000.f )
        {
            return "move";
        }
        return specialCooldown > 0.f ? (attackCooldown > 0.f ? "" : "attack") : "special";
    });
    moveState.mUpdate = [](Shade::Entity* AIEntity, float deltaSeconds) {
        if (attackCooldown > 0.f)
        {
            attackCooldown -= deltaSeconds;
        }
        if (specialCooldown > 0.f)
        {
            specialCooldown -= deltaSeconds;
        }

        LocomotionComponent* locomotion = AIEntity->GetComponent<LocomotionComponent>();
        if (locomotion == nullptr)
        {
            Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
            logService->LogError("Expected locomotion component for AI");
            return;
        }
        Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
        locomotion->mMovingRight = player->GetPositionX() > AIEntity->GetPositionX();
        locomotion->mMovingLeft = player->GetPositionX() < AIEntity->GetPositionX();
        locomotion->mMovingUp = player->GetPositionY() > AIEntity->GetPositionY();
        locomotion->mMovingDown = player->GetPositionY() < AIEntity->GetPositionY();
    };
    moveState.mTransitions.push_back([](Shade::Entity* AIEntity){
        StaggerComponent* stagger = AIEntity->GetComponent<StaggerComponent>();
        if (stagger && stagger->IsStaggering())
        {
            return "";
        }

        Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
        const float diff_x = AIEntity->GetPositionX() - player->GetPositionX();
        const float diff_y = AIEntity->GetPositionY() - player->GetPositionY();
        return (diff_x * diff_x + diff_y * diff_y) < 40000.f ? (specialCooldown > 0.f ? (attackCooldown > 0.f ? "idle" : "attack") : "special") : "";
    });
    attackState.mUpdate = [](Shade::Entity* AIEntity, float deltaSeconds) {
    };
    attackState.mOnEnter = [](Shade::Entity* AIEntity){
        attackCooldown = 1.5f;

        Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
        AttackComponent* attackComponent = AIEntity->GetComponent<AttackComponent>();

        const bool isRight = player->GetPositionX() > AIEntity->GetPositionX();
        bool attacked = attackComponent->TryDoAttack(isRight ? "attack_right" : "attack_left");
        if (attacked)
        {
            FacingComponent* facing = AIEntity->GetComponent<FacingComponent>();
            facing->mDirection = isRight ? FacingDirection::RIGHT : FacingDirection::LEFT;
        }
    };
    attackState.mTransitions.push_back([](Shade::Entity* AIEntity) {
        AttackComponent* attackComponent = AIEntity->GetComponent<AttackComponent>();
        return attackComponent->IsDoingAttack() ? "" : "idle";
    });
    specialState.mUpdate = [](Shade::Entity* AIEntity, float deltaSeconds) {
    };
    specialState.mOnEnter = [](Shade::Entity* AIEntity){
        specialCooldown = 1.5f;

        Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
        AttackComponent* attackComponent = AIEntity->GetComponent<AttackComponent>();

        bool attacked = attackComponent->TryDoAttack("special");
    };
    specialState.mTransitions.push_back([](Shade::Entity* AIEntity) {
        AttackComponent* attackComponent = AIEntity->GetComponent<AttackComponent>();
        return attackComponent->IsDoingAttack() ? "" : "idle";
    });
    std::unordered_map<std::string, AIState> stateInfo;
    stateInfo["idle"] = idleState;
    stateInfo["move"] = moveState;
    stateInfo["attack"] = attackState;
    stateInfo["special"] = specialState;
    
    return new StateMachineAIComponent("idle", stateInfo); 
}

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