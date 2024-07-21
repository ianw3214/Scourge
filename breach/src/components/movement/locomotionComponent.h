#pragma once

#include "shade/game/entity/component/component.h"

#include <string>

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
    LocomotionComponent(const std::string& idleLeftAnim, const std::string& idleRightAnim, const std::string& runLeftAnim, const std::string& runRightAnim) 
        : mIdleLeftAnim(idleLeftAnim), mIdleRightAnim(idleRightAnim), mMoveLeftAnim(runLeftAnim), mMoveRightAnim(runRightAnim) {}
    LocomotionComponent(float speed, const std::string& idleLeftAnim, const std::string& idleRightAnim, const std::string& runLeftAnim, const std::string& runRightAnim) 
        : mSpeed(speed), mIdleLeftAnim(idleLeftAnim), mIdleRightAnim(idleRightAnim), mMoveLeftAnim(runLeftAnim), mMoveRightAnim(runRightAnim) {}
    
    void Update(float deltaSeconds) override;

    void DisableLocomotion();
    void EnableLocomotion();
private:
    std::string mIdleLeftAnim = "idle_left";
    std::string mIdleRightAnim = "idle_right";
    std::string mMoveLeftAnim = "run_left";
    std::string mMoveRightAnim = "run_right";
};
