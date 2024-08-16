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
    static const std::string ComponentID;
#ifdef BUILD_BREACH_EDITOR
public:
    virtual const char* GetComponentID() const { return ComponentID.c_str(); }
    virtual const char* GetDisplayName() const override { return "Facing Component"; }
#endif
public:
    FacingDirection mDirection = FacingDirection::RIGHT;
};