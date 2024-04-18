#include "attackComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

#include "debug/util.h"
#include "components/hitboxComponent.h"
#include "components/healthComponent.h"

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
void AttackComponent::RegisterAttacksToAnimFrames(const std::vector<std::pair<std::string, uint16_t>>& attackAnimData)
{
    Shade::AnimatedSpriteComponent* anim = mEntityRef->GetCachedAnimatedSprite();
    if (anim == nullptr)
    {
        Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        logService->LogWarning("Tried to register attacks to entity without animated sprite component");
        return;
    }
    for (const auto& pair : attackAnimData)
    {
        const std::string& name = pair.first;
        anim->mEvents[pair.second] = [this, name](Shade::Entity* triggerEntity) {
            this->DoAttack(name);
        };
    }
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
    const AttackInfo& attackInfo = it->second;
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
    DebugUtils::DrawDebugRectOutline(Shade::Vec2{ mEntityRef->GetPositionX() + attackInfo.mOffsetX, mEntityRef->GetPositionY() + attackInfo.mOffsetY}, attackInfo.mWidth, attackInfo.mHeight, Shade::Colour{ 0.15f, 0.15f, 0.15f}, 0.5f);
    return true;
}