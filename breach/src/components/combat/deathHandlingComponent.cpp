#include "deathHandlingComponent.h"

#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/game/entity/entity.h"

#include "components/facingComponent.h"

// ======================================
DeathHandlingComponent* DeathHandlingComponent::LoadFromFileHandle(Shade::KeyValueHandle handle)
{
    if (handle.IsValid())
    {
        // TODO: Sync up the default values
        std::string dieLeftAnim = "die_left";
        std::string dieRightAnim = "die_right";
        while (handle.IsValid())
        {
            if (handle.GetKey() == "die_left")
            {
                dieLeftAnim = handle.TryGetString();
            }
            if (handle.GetKey() == "die_right")
            {
                dieRightAnim = handle.TryGetString();
            }
            handle.ToNext();
        }

        return new DeathHandlingComponent(dieLeftAnim, dieRightAnim);
    }   

    return new DeathHandlingComponent(); 
}

// ======================================
DeathHandlingComponent::DeathHandlingComponent()
{

}

// ======================================
DeathHandlingComponent::DeathHandlingComponent(const std::string& dieLeftAnim, const std::string& dieRightAnim)
    : mDieLeftAnim(dieLeftAnim), mDieRightAnim(dieRightAnim)
{

}

// ======================================
void DeathHandlingComponent::Update(float deltaSeconds) 
{
    if (mDeleteTimer > 0.f)
    {
        mDeleteTimer -= deltaSeconds;
        if (mDeleteTimer <= 0.f)
        {
            mEntityRef->MarkForDelete();
        }
    }
}

// ======================================
void DeathHandlingComponent::HandleDeath(float deathTime)
{
    mDeleteTimer = deathTime;

    FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
    mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? mDieRightAnim : mDieLeftAnim);
}