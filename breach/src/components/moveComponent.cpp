#include "moveComponent.h"

#include "components/facingComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"

// ======================================
void BaseMovementComponent::Update(float deltaSeconds)
{
    FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
        if (mDisable)
        {
            return;
        }
        FacingDirection NewFacingDir = facing->mDirection;
        bool bMoving = false;
        if (mMovingUp)
        {
            mEntityRef->SetPositionY(mEntityRef->GetPositionY() + mSpeed * deltaSeconds);
            bMoving = true;
        }
        if (mMovingDown)
        {
            mEntityRef->SetPositionY(mEntityRef->GetPositionY() - mSpeed * deltaSeconds);
            bMoving = true;
        }
        if (mMovingRight)
        {
            mEntityRef->SetPositionX(mEntityRef->GetPositionX() + mSpeed * deltaSeconds);
            bMoving = true;
            NewFacingDir = FacingDirection::RIGHT;
        }
        if (mMovingLeft)
        {
            mEntityRef->SetPositionX(mEntityRef->GetPositionX() - mSpeed * deltaSeconds);
            bMoving = true;
            NewFacingDir = FacingDirection::LEFT;
        }
        // Update animation if any of the movement related states were changed
        if (NewFacingDir != facing->mDirection || bMoving != mWasMoving)
        {
            mWasMoving = bMoving;
            facing->mDirection = NewFacingDir;
            if (bMoving)
            {
                mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? "run_right" : "run_left");
            }
            else
            {
                mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? "idle_right" : "idle_left");
            }
        }
        // Reset movement flags after using them
        mMovingLeft = false;
        mMovingRight = false;
        mMovingUp = false;
        mMovingDown = false;
}

// ======================================
void BaseMovementComponent::DisableMovement()
{
    mDisable = true;
    mWasMoving = false;  // TODO: Hacky... fix soon
}

// ======================================
void BaseMovementComponent::EnableMovement()
{
    mDisable = false;
}