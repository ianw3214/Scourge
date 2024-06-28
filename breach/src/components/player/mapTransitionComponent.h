#pragma once

#include "shade/game/entity/component/component.h"

// This component marks the entities that can control map flow and when a new map should be loaded
//  - In a single player setting, this should only be put on the player
//  - For multiplayer settings, this can wait for all players to be in the same zone before proceeding
class MapTransitionComponent : public Shade::Component
{
public:
    void Update(float deltaSeconds) override;
};