#pragma once

#include "shade/game/entity/component/component.h"

#include <unordered_map>
#include <vector>
#include <string>

// ======================================
// This could be generalized more instead of hard-coded in the future if necessary
//  - perhaps some sort of "team" system could suffice
enum class AttackTarget {
    PLAYER,
    ENEMY
};

// ======================================
struct AttackHitInfo {
    float mOffsetX = 0.f;
    float mOffsetY = 0.f;
    float mWidth = 10.f;
    float mHeight = 10.f;

    float mDamage = 1.f;
    AttackTarget mTarget = AttackTarget::ENEMY;

    AttackHitInfo() = default;
    AttackHitInfo(float offsetX, float offsetY, float width, float height, float damage, AttackTarget target)
        : mOffsetX(offsetX), mOffsetY(offsetY), mWidth(width), mHeight(height), mDamage(damage), mTarget(target) {}
};

// ======================================
struct AttackInfo {
    std::string mAnimation;
    bool mDisableMovement = true;

    // TODO: This can be timed to exactly when the animation ends if desired
    float mDuration = 0.f;

    // TODO: This should be in a container in the future
    //  so that each attack can support multiple "events"
    uint32_t mTriggerFrame;
    AttackHitInfo mHitInfo;

    AttackInfo() = default;
    AttackInfo(const std::string& anim, bool disableMovement, float duration, uint32_t triggerFrame, AttackHitInfo hitInfo)
        : mAnimation(anim), mDisableMovement(disableMovement), mDuration(duration), mTriggerFrame(triggerFrame), mHitInfo(hitInfo) {}
};

// ======================================
// Entity attacks are stored via a map with the name of the attack mapping to the attack data
class AttackComponent : public Shade::Component {
public:
    AttackComponent();

    void RegisterAttackInfo(const std::string& name, const AttackInfo& attack);
    // This must be called after the animated sprite component has been set up
    void RegisterAttacksToAnimFrames();

    void Update(float deltaSeconds) override;

    bool IsDoingAttack() const;
    
    bool DoAttack(const std::string& name);
    bool TriggerAttackHitEvent(const std::string& name);

private:
    std::unordered_map<std::string, AttackInfo> mAttackMap;

    std::string mCurrentAttack;
    float mCurrentAttackTimer = 0.f;
    
};