#include "locomotionComponent.h"

#include "components/facingComponent.h"
#include "components/movement/moveComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"

// ======================================
void LocomotionComponent::Update(float deltaSeconds)
{
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
        mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? "run_right" : "run_left", false);
    }
    else
    {
        mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? "idle_right" : "idle_left", false);
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