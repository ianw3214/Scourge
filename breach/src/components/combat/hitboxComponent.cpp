#include "hitboxComponent.h"

#include "shade/game/entity/entity.h"

#include <imgui/imgui.h>

#ifdef BUILD_BREACH_EDITOR
// ======================================
void HitboxComponent::ShowImguiDetails() 
{
    ImGui::DragFloat("width", &mWidth, 1.f, 0.f, 1000.f);
    ImGui::DragFloat("height", &mHeight, 1.f, 0.f, 1000.f);
}
#endif

// ======================================
HitboxComponent* HitboxComponent::LoadFromFileHandle(Shade::KeyValueHandle handle)
{
    float width = HitboxComponentDefaults::width;
    float height = HitboxComponentDefaults::height;

    while (handle.IsValid())
    {
        if (handle.GetKey() == "width")
        {
            width = handle.TryGetFloat(width);
        }
        if (handle.GetKey() == "height")
        {
            height = handle.TryGetFloat(height);
        }
        handle.ToNext();
    }

    return new HitboxComponent(width, height);
}

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
bool HitboxComponent::Intersects(const Shade::Box& other) const
{
    return Shade::BoxIntersectsBox(GetBox(), other);
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