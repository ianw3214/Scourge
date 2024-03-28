#include "hitboxComponent.h"

#include "shade/game/entity/entity.h"

// ======================================
HitboxComponent::HitboxComponent(float width, float height)
    : mWidth(width)
    , mHeight(height)
{

}

// ======================================
float HitboxComponent::GetWidth() const
{
    return mWidth;
}

// ======================================
float HitboxComponent::GetHeight() const
{
    return mHeight;
}

// ======================================
Shade::Vec2 HitboxComponent::GetBottomLeftPos() const
{
    return Shade::Vec2{ mEntityRef->GetPositionX() - mWidth / 2.f, mEntityRef->GetPositionY() };
}
