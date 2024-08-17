#pragma once

#include "shade/game/entity/component/component.h"

#include "shade/file/keyValueFile.h"

#include <string>

// ======================================
namespace LocomotionComponentDefaults {
    const std::string idleLeftAnim = "idle_left";
    const std::string idleRightAnim = "idle_right";
    const std::string moveLeftAnim = "run_left";
    const std::string moveRightAnim = "run_right";
    constexpr float speed = 200.f;
}

// ======================================
// The locomotion component handles various basic movement states of an entity
//  - Hooking up animations to various movement states is done in the locomotion component
//  - The decision on HOW to move is still done by other controlling components
class LocomotionComponent : public Shade::Component
{
public:
    static const std::string ComponentID;
#ifdef BUILD_BREACH_EDITOR
public:
    virtual const char* GetComponentID() const { return ComponentID.c_str(); }
    virtual const char* GetDisplayName() const override { return "Locomotion Component"; }
    virtual void ShowImguiDetails() override;
    virtual void SaveToKeyValueFile(Shade::KeyValueFile& file) const override;
#endif
public:
    static LocomotionComponent* LoadFromFileHandle(Shade::KeyValueHandle handle);
public:
    bool mEnable = true;
    
    bool mMovingLeft = false;
    bool mMovingRight = false;
    bool mMovingUp = false;
    bool mMovingDown = false;

    // These are configuration variables to set movement properties
    float mSpeed = LocomotionComponentDefaults::speed;
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
    std::string mIdleLeftAnim = LocomotionComponentDefaults::idleLeftAnim;
    std::string mIdleRightAnim = LocomotionComponentDefaults::idleRightAnim;
    std::string mMoveLeftAnim = LocomotionComponentDefaults::moveLeftAnim;
    std::string mMoveRightAnim = LocomotionComponentDefaults::moveRightAnim;
};
