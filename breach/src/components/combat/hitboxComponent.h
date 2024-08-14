#pragma once

#include "shade/game/entity/component/component.h"

#include "shade/common/collision.h"
#include "shade/common/vec.h"
#include "shade/file/keyValueFile.h"

// ======================================
namespace HitboxComponentDefaults {
    constexpr float width = 10.f;
    constexpr float height = 10.f;
}

// ======================================
// Represents the hit box of an entity, used to test against attacks hitting a character
//  - Currently assumes bottom middle of the box to be the position of the entity
//      - This may need to be adjusted to be a custom offset system in the future
class HitboxComponent : public Shade::Component
{
#ifdef BUILD_BREACH_EDITOR
public:
    const char* GetDisplayName() const override { return "Hitbox Component"; }
    void ShowImguiDetails() override;
#endif
public:
    static HitboxComponent* LoadFromFileHandle(Shade::KeyValueHandle handle);
public:
    HitboxComponent(float width, float height);

    float GetWidth() const;
    float GetHeight() const;
    Shade::Vec2 GetBottomLeftPos() const;
    Shade::Box GetBox() const;

    bool Intersects(const Shade::Box& other) const;
    bool Intersects(const HitboxComponent& other) const;

private:
    float mWidth = HitboxComponentDefaults::width;
    float mHeight = HitboxComponentDefaults::height;
};