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

// ======================================
Shade::Box HitboxComponent::GetBox() const
{
    return Shade::Box{ GetBottomLeftPos(), mWidth, mHeight };
}

// ======================================
bool HitboxComponent::Intersects(const HitboxComponent& other) const
{
    // Never hit ourselves
    if (&other == this)
    {
        return false;
    }
    return Shade::BoxIntersectsBox(GetBox(), other.GetBox());
}