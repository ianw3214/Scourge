#include "attackComponent.h"

#include "definitions.h"

#include "shade/game/entity/entity.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/graphics/flare/flareService.h"
#include "shade/graphics/texture.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
#include "shade/resource/manager.h"

#include "debug/util.h"
#include "components/combat/hitboxComponent.h"
#include "components/combat/healthComponent.h"
#include "components/facingComponent.h"
#include "components/movement/locomotionComponent.h"
#include "components/movement/moveComponent.h"
#include "components/movement/staggerComponent.h"

// TODO: Remove - Temporary for determining player or AI
#include "components/ai/stateMachineAIComponent.h"

// ======================================
AttackComponent::AttackComponent()
    : mAttackMap()
{
    
}

// ======================================
AttackInfo& AttackComponent::RegisterAttackInfo(const std::string& name, const AttackInfo& attack)
{
    // TODO: Check against duplicate attack registration?
    mAttackMap[name] = attack;

    return mAttackMap[name];
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
            // TODO: Not all attacks will disable stagger - need to set in attack
            if (StaggerComponent* stagger = mEntityRef->GetComponent<StaggerComponent>())
            {
                stagger->EnableStagger();
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

// ======================================
//  - TODO: Why does this return a bool? Either define it well or just turn this into void
bool AttackComponent::TriggerAttackHitEvent(const AttackHitInfo& attackInfo)
{
    for (const AttackHitBox& attackHitBox : attackInfo.mAttackBoxes)
    {
        Shade::Box attackBox = Shade::Box(Shade::Vec2{ mEntityRef->GetPositionX() + attackHitBox.mOffsetX, mEntityRef->GetPositionY() + attackHitBox.mOffsetY}, attackHitBox.mWidth, attackHitBox.mHeight);

        // Spawn vfx if specified by the attack
        if (attackHitBox.mEffectTexture.IsValid())
        {
            // TODO: Working on this...
            Shade::FlareService* flareVFX = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::FlareService>();
            Shade::Vec2 vfxPosition = attackBox.mPosition;
            vfxPosition.x += attackHitBox.mEffectOffsetX;
            vfxPosition.y += attackHitBox.mEffectOffsetY;
            // TODO: Adjustable render layers?
            // TODO: Adjustable vfx lifetime
            // TODO: Don't want to manually specify width/height here
            flareVFX->SpawnEffect(Shade::Effect{ vfxPosition, 80.f, 140.f, attackHitBox.mEffectTexture, static_cast<int>(RenderLayer::DEFAULT), 0.25f});
        }
        // Check the actual hit box for hits on enemies
        for (const auto& entity : mEntityRef->GetWorldEntities())
        {
            // The attack has already hit the entity, don't hit it twice
            if (std::find(mCurrentHitEnemies.begin(), mCurrentHitEnemies.end(), entity.get()) != mCurrentHitEnemies.end())
            {
                continue;
            }

            HealthComponent* health = entity->GetComponent<HealthComponent>();
            if (health && health->IsDead())
            {
                continue;
            }
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
                    if (health != nullptr)
                    {
                        health->DecrementHealth(attackInfo.mDamage);

                        // TODO: More thought needs to be put into the stagger design
                        //  - Certain enemies may not want to be staggered (bosses, gigantic enemies, etc)
                        //      - Boss staggers can be implemented as a short timed delay rather than full animation
                        //  - A "weight class" system can be used to determine stagger times/triggers
                        //  - Some attacks also may not want to stagger enemies or do so at different times (light attacks)
                        //  - Stagger direction also needs to be accounted for here
                        if (StaggerComponent* stagger = entity->GetComponent<StaggerComponent>())
                        {
                            // TODO: Replace hard-coded number - embed into attack data
                            stagger->TryStaggerInDirection(0.5f, mCurrentAttackFacing == FacingDirection::RIGHT ? FacingDirection::LEFT : FacingDirection::RIGHT);
                        }
                    }
                    
                    mCurrentHitEnemies.emplace_back(entity.get());

                    // TODO: This should be data-driven by the attack
                    // TODO: Better calculation of where the vfx should play
                    Shade::Vec2 vfxPosition = attackBox.mPosition;
                    vfxPosition.x += attackBox.mWidth / 2.f - (mCurrentAttackFacing == FacingDirection::RIGHT ? 0.f : 40.f);
                    vfxPosition.y += attackBox.mHeight / 2.f;
                    Shade::ResourceManager* resourceManager = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ResourceManager>();
                    Shade::ResourceHandle hardCodedHandle  = resourceManager->LoadResource<Shade::Texture>(mCurrentAttackFacing == FacingDirection::RIGHT ? "assets/breach/VFX/whack_right.png" : "assets/breach/VFX/whack_left.png");
                    Shade::FlareService* flareVFX = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::FlareService>();
                    flareVFX->SpawnEffect(Shade::Effect{ vfxPosition, 40.f, 30.f, hardCodedHandle, static_cast<int>(RenderLayer::VFX_TOP), 0.25f});
                }
            }
        }
#ifdef DEBUG_BREACH
        DebugUtils::DrawDebugRectOutline(Shade::Vec2{ mEntityRef->GetPositionX() + attackHitBox.mOffsetX, mEntityRef->GetPositionY() + attackHitBox.mOffsetY}, attackHitBox.mWidth, attackHitBox.mHeight, Shade::Colour{ 0.15f, 0.15f, 0.15f}, 0.5f);
#endif
    }
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
    mCurrentHitEnemies.clear();
    mCurrentAttackTimer = attackInfo.mDuration;
    mCurrentAttackFacing = facing->mDirection;

    // TODO: Not all attacks will disable stagger - need to set in attack`
    if (StaggerComponent* stagger = mEntityRef->GetComponent<StaggerComponent>())
    {
        stagger->DisableStagger();
    }

    return true;
}