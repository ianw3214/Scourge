#pragma once

#include "shade/game/entity/component/component.h"

#include "shade/common/collision.h"
#include "shade/common/vec.h"

// Represents the hit box of an entity, used to test against attacks hitting a character
//  - Currently assumes bottom middle of the box to be the position of the entity
//      - This may need to be adjusted to be a custom offset system in the future
class HitboxComponent : public Shade::Component
{
public:
    HitboxComponent(float width, float height);

    float GetWidth() const;
    float GetHeight() const;
    Shade::Vec2 GetBottomLeftPos() const;
    Shade::Box GetBox() const;

    bool Intersects(const Shade::Box& other) const;
    bool Intersects(const HitboxComponent& other) const;

private:
    float mWidth = 10.f;
    float mHeight = 10.f;
};