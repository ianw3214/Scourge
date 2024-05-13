#include "attackComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

#include "debug/util.h"
#include "components/combat/hitboxComponent.h"
#include "components/combat/healthComponent.h"
#include "components/facingComponent.h"
#include "components/movement/moveComponent.h"
#include "components/movement/locomotionComponent.h"

// TODO: Remove - Temporary for determining player or AI
#include "components/ai/stateMachineAIComponent.h"

// ======================================
AttackComponent::AttackComponent()
    : mAttackMap()
{
    
}

// ======================================
void AttackComponent::RegisterAttackInfo(const std::string& name, const AttackInfo& attack)
{
    mAttackMap[name] = attack;
}

// ======================================
void AttackComponent::RegisterAttacksToAnimFrames()
{
    Shade::AnimatedSpriteComponent* anim = mEntityRef->GetCachedAnimatedSprite();
    if (anim == nullptr)
    {
        Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        logService->LogWarning("Tried to register attacks to entity without animated sprite component");
        return;
    }
    for (const auto& pair : mAttackMap)
    {
        for (const AttackHitInfo& attackHitInfo : pair.second.mHitInfo)
        {
            const uint32_t attackHitFrame = attackHitInfo.mTriggerFrame;
            anim->mEvents[attackHitFrame] = [this, attackHitInfo](Shade::Entity* triggerEntity) {
                this->TriggerAttackHitEvent(attackHitInfo);
            };
        }
    }
}

// ======================================
void AttackComponent::Update(float deltaSeconds)
{
    if (!mCurrentAttack.empty())
    {
        // TODO: Add an assert that current attack exists in the map
        const AttackInfo& attackInfo = mAttackMap[mCurrentAttack];
        if (attackInfo.mMoveSpeed > 0.f)
        {
            BaseMovementComponent* movement = mEntityRef->GetComponent<BaseMovementComponent>();
            if (mCurrentAttackFacing == FacingDirection::LEFT)
            {
                movement->MoveLeft(attackInfo.mMoveSpeed * deltaSeconds);
            }
            if (mCurrentAttackFacing == FacingDirection::RIGHT)
            {
                movement->MoveRight(attackInfo.mMoveSpeed * deltaSeconds);
            }
        }
        mCurrentAttackTimer -= deltaSeconds;
        if (mCurrentAttackTimer <= 0.f)
        {
            mCurrentAttack.clear();
            mCurrentAttackTimer = 0.f;
            if (attackInfo.mDisableMovement)
            {
                LocomotionComponent* locomotion = mEntityRef->GetComponent<LocomotionComponent>();
                locomotion->EnableLocomotion();
            }
            if (attackInfo.mInvulnerable)
            {
                HealthComponent* health = mEntityRef->GetComponent<HealthComponent>();
                health->mIsInvulnerable = false;
            }
        }
    }
}

// ======================================
bool AttackComponent::IsDoingAttack() const
{
    return !mCurrentAttack.empty();
}

// ======================================
bool AttackComponent::TryDoAttack(const std::string& name)
{
    if (IsDoingAttack())
    {
        return false;
    }
    return DoAttack(name);
}

bool AttackComponent::TriggerAttackHitEvent(const AttackHitInfo& attackInfo)
{
    Shade::Box attackBox = Shade::Box(Shade::Vec2{ mEntityRef->GetPositionX() + attackInfo.mOffsetX, mEntityRef->GetPositionY() + attackInfo.mOffsetY}, attackInfo.mWidth, attackInfo.mHeight);
    for (const auto& entity : mEntityRef->GetWorldEntities())
    {
        if (entity.get() == mEntityRef)
        {
            continue;
        }
        // TODO: Need an ACTUAL way to determine if an entity is a player or enemy
        if (attackInfo.mTarget == AttackTarget::PLAYER && entity->GetComponent<StateMachineAIComponent>() != nullptr)
        {
            continue;
        }
        if (attackInfo.mTarget == AttackTarget::ENEMY && entity->GetComponent<StateMachineAIComponent>() == nullptr)
        {
            continue;
        }
        if (HitboxComponent* hitbox = entity->GetComponent<HitboxComponent>())
        {
            if (hitbox->Intersects(attackBox))
            {
                if (HealthComponent* health = entity->GetComponent<HealthComponent>())
                {
                    health->DecrementHealth(attackInfo.mDamage);
                }
            }
        }
    }
#ifdef DEBUG_BREACH
    DebugUtils::DrawDebugRectOutline(Shade::Vec2{ mEntityRef->GetPositionX() + attackInfo.mOffsetX, mEntityRef->GetPositionY() + attackInfo.mOffsetY}, attackInfo.mWidth, attackInfo.mHeight, Shade::Colour{ 0.15f, 0.15f, 0.15f}, 0.5f);
#endif
    return true;
}

// ======================================
bool AttackComponent::DoAttack(const std::string& name)
{
    auto it = mAttackMap.find(name);
    if (it == mAttackMap.end())
    {
        Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        logService->LogWarning("Tried to do an attack that doesn't exist: " + name);
        return false;
    }
    FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
    if (!facing)
    {
        Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        logService->LogError("Missing facing component on entity with attack component");
        return false;
    }
    const AttackInfo& attackInfo = it->second;
    mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(attackInfo.mAnimation);
    if (attackInfo.mDisableMovement)
    {
        LocomotionComponent* locomotion = mEntityRef->GetComponent<LocomotionComponent>();
        locomotion->DisableLocomotion();
    }
    if (attackInfo.mInvulnerable)
    {
        HealthComponent* health = mEntityRef->GetComponent<HealthComponent>();
        health->mIsInvulnerable = true;
    }
    mCurrentAttack = name;
    mCurrentAttackTimer = attackInfo.mDuration;
    mCurrentAttackFacing = facing->mDirection;
    return true;
}