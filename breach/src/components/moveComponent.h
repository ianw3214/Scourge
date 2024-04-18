#pragma once

#include "shade/game/entity/component/component.h"

class BaseMovementComponent : public Shade::Component
{
public:
    bool mWasMoving = false;
    bool mDisable = false;
    
    // These variables are directly set to control the movement
    // TODO: Consider when to reset these variables, or if they should even be reset per frame?
    bool mMovingLeft = false;
    bool mMovingRight = false;
    bool mMovingUp = false;
    bool mMovingDown = false;

    // These are configuration variables to set movement properties
    float mSpeed = 200.f;
public:
    // ======================================
    BaseMovementComponent() = default;
    BaseMovementComponent(float speed) : mSpeed(speed) {}
    
    void Update(float deltaSeconds) override;

    void DisableMovement();
    void EnableMovement();
};
