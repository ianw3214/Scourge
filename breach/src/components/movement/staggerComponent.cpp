#include "staggerComponent.h"

#include "components/facingComponent.h"

#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/game/entity/entity.h"

// ======================================
StaggerComponent::StaggerComponent()
{

}

// ======================================
void StaggerComponent::Update(float deltaSeconds) 
{
    if (mStaggerTimer > 0.f)
    {
        FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
        mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? "stagger_right" : "stagger_left", false);

        mStaggerTimer -= deltaSeconds;
    }
}

// ======================================
bool StaggerComponent::IsStaggering() const
{
    return mStaggerTimer > 0.f;
}

// ======================================
void StaggerComponent::TryStagger(float time)
{
    if (!mCanStagger)
    {
        return;
    }

    mStaggerTimer = time;

    FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
    mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? "stagger_right" : "stagger_left", false);

    // TODO: Potentially handle inturrupting actions here as well
    //  - e.g. inturrupted attack
}

// ======================================
// TODO: Potentially refactor to share common code w/ above function
//  - Alternatively, have the attacks handle changing the entities direction and just use the normal TryStagger
void StaggerComponent::TryStaggerInDirection(float time, FacingDirection direction)
{
    if (!mCanStagger)
    {
        return;
    }

    mStaggerTimer = time;

    mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(direction == FacingDirection::RIGHT ? "stagger_right" : "stagger_left", false);
    // Update the facing to face the direction that the stagger is in
    FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
    facing->mDirection = direction;

    // TODO: Potentially handle inturrupting actions here as well
    //  - e.g. inturrupted attack
}

// ======================================
void StaggerComponent::DisableStagger()
{
    mCanStagger = false;
}

// ======================================
void StaggerComponent::EnableStagger()
{
    mCanStagger = true;
}