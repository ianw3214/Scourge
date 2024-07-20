#pragma once

#include "shade/game/entity/component/component.h"

#include <string>

// Handles the death of an entity
//  - By default, play a specified animation and mark the entity for remove after a timer
class DeathHandlingComponent : public Shade::Component
{
public:
    DeathHandlingComponent();
    DeathHandlingComponent(const std::string& mDieLeftAnim, const std::string& mDieRightAnim);

    void Update(float deltaSeconds) override;

    void HandleDeath(float deathTime);

private:
    float mDeleteTimer = 0.f;

    std::string mDieLeftAnim = "die_left";
    std::string mDieRightAnim = "die_right";
};