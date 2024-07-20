#include "locomotionComponent.h"

#include "components/facingComponent.h"
#include "components/combat/healthComponent.h"
#include "components/movement/moveComponent.h"
#include "components/movement/staggerComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"

// ======================================
void LocomotionComponent::Update(float deltaSeconds)
{
    HealthComponent* health = mEntityRef->GetComponent<HealthComponent>();
    if (health && health->IsDead())
    {
        return;
    }

    StaggerComponent* stagger = mEntityRef->GetComponent<StaggerComponent>();
    if (stagger && stagger->IsStaggering())
    {
        return;
    }

    FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
    BaseMovementComponent* move = mEntityRef->GetComponent<BaseMovementComponent>();
    if (!mEnable)
    {
        return;
    }
    bool bMoving = false;
    if (mMovingUp && !mMovingDown)
    {
        move->MoveUp(mSpeed * deltaSeconds);
        bMoving = true;
    }
    if (mMovingDown && !mMovingUp)
    {
        move->MoveDown(mSpeed * deltaSeconds);
        bMoving = true;
    }
    if (mMovingRight && !mMovingLeft)
    {
        move->MoveRight(mSpeed * deltaSeconds);
        bMoving = true;
        facing->mDirection = FacingDirection::RIGHT;
    }
    if (mMovingLeft && !mMovingRight)
    {
        move->MoveLeft(mSpeed * deltaSeconds);
        bMoving = true;
        facing->mDirection = FacingDirection::LEFT;
    }
    if (bMoving)
    {
        mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? mMoveRightAnim : mMoveLeftAnim, false);
    }
    else
    {
        mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? mIdleRightAnim : mIdleLeftAnim, false);
    }
    // Reset movement flags after using them
    mMovingLeft = false;
    mMovingRight = false;
    mMovingUp = false;
    mMovingDown = false;
}

// ======================================
void LocomotionComponent::DisableLocomotion()
{
    mEnable = false;
}

// ======================================
void LocomotionComponent::EnableLocomotion()
{
    mEnable = true;
}