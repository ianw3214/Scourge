#include "deathHandlingComponent.h"

#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/game/entity/entity.h"

#include "components/facingComponent.h"

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

    // TODO: Allow the animations to be set rather than hard-coding the strings here
    FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
    mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? mDieRightAnim : mDieLeftAnim);
}