#pragma once
#ifdef DEBUG_BREACH

#include "debug/util.h"
#include "shade/game/entity/component/component.h"
#include "shade/game/entity/entity.h"

#include "components/healthComponent.h"
#include "components/hitboxComponent.h"

class BasicDebugComponent : public Shade::Component
{
public:
    // ======================================
    void Update(float deltaSeconds) override {
        // Draw the position of the entity
        // TODO: Draw this with a sphere shape
        // DebugUtils::DrawDebugLine(Shade::Vec2{ mEntityRef->GetPositionX(), mEntityRef->GetPositionY()}, 10.f);

        // Draw entity health if applicable
        HealthComponent* health = mEntityRef->GetComponent<HealthComponent>();
        if (health)
        {
            const float x = mEntityRef->GetPositionX() - 30.f;
            const float y = mEntityRef->GetPositionY() - 20.f;
            const float width = 60.f * health->getCurrentHealth() / health->getMaxHealth();
            DebugUtils::DrawDebugRect(Shade::Vec2{ x, y }, 60.f, 10.f, Shade::Colour{ 0.15f, 0.15f, 0.15f});
            // TODO: Differentiate debug health colours between player and enemies
            DebugUtils::DrawDebugRect(Shade::Vec2{ x, y }, width, 10.f, Shade::Colour{ 1.f, 0.f, 0.f });
        }

        // Draw hitbox if applicable
        HitboxComponent* hitbox = mEntityRef->GetComponent<HitboxComponent>();
        if (hitbox)
        {
            const Shade::Vec2 bottomLeft = hitbox->GetBottomLeftPos();
            const float width = hitbox->GetWidth();
            const float height = hitbox->GetHeight();
            const Shade::Vec2 topRight = bottomLeft.Right(width).Up(height);
            DebugUtils::DrawDebugLine(bottomLeft, bottomLeft.Right(width));
            DebugUtils::DrawDebugLine(bottomLeft, bottomLeft.Up(height));
            DebugUtils::DrawDebugLine(topRight.Left(width), topRight);
            DebugUtils::DrawDebugLine(topRight.Down(height), topRight);
        }
    }
};

#endif