#include "moveComponent.h"

#include "shade/game/entity/entity.h"

// ======================================
void BaseMovementComponent::MoveLeft(float distance)
{
    mEntityRef->SetPositionX(mEntityRef->GetPositionX() - distance);
}

// ======================================
void BaseMovementComponent::MoveRight(float distance)
{
    mEntityRef->SetPositionX(mEntityRef->GetPositionX() + distance);
}

// ======================================
void BaseMovementComponent::MoveUp(float distance)
{
    mEntityRef->SetPositionY(mEntityRef->GetPositionY() + distance);
}

// ======================================
void BaseMovementComponent::MoveDown(float distance)
{
    mEntityRef->SetPositionY(mEntityRef->GetPositionY() - distance);
}