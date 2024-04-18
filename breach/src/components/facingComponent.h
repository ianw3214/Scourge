#pragma once

#include "shade/game/entity/component/component.h"

// ======================================
enum class FacingDirection {
    LEFT,
    RIGHT
};

class FacingComponent : public Shade::Component
{
public:
    FacingDirection mDirection = FacingDirection::RIGHT;
};