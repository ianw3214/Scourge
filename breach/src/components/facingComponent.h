#pragma once

#include "shade/game/entity/component/component.h"

// ======================================
enum class FacingDirection {
    LEFT,
    RIGHT
};

// This would serve well to just be part of the entity
//  - Perhaps a custom entity class can be used based on the base shade entity
class FacingComponent : public Shade::Component
{
public:
    FacingDirection mDirection = FacingDirection::RIGHT;
};