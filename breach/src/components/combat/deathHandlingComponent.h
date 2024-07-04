#pragma once

#include "shade/game/entity/component/component.h"

// Handles the death of an entity
//  - By default, play a specified animation and mark the entity for remove after a timer
class DeathHandlingComponent : public Shade::Component
{
public:
    DeathHandlingComponent();

    void Update(float deltaSeconds) override;

    void HandleDeath(float deathTime);

private:
    float mDeleteTimer = 0.f;
};