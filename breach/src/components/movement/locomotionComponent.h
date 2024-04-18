#pragma once

#include "shade/game/entity/component/component.h"

// The locomotion component handles various basic movement states of an entity
//  - Hooking up animations to various movement states is done in the locomotion component
//  - The decision on HOW to move is still done by other controlling components
class LocomotionComponent : public Shade::Component
{
public:
    bool mEnable = true;
    
    bool mMovingLeft = false;
    bool mMovingRight = false;
    bool mMovingUp = false;
    bool mMovingDown = false;

    // These are configuration variables to set movement properties
    float mSpeed = 200.f;
public:
    // ======================================
    LocomotionComponent() = default;
    LocomotionComponent(float speed) : mSpeed(speed) {}
    
    void Update(float deltaSeconds) override;

    void DisableLocomotion();
    void EnableLocomotion();
};
