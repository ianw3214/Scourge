#pragma once

#include "shade/game/entity/component/component.h"

#include "components/facingComponent.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <limits>

// ======================================
// This could be generalized more instead of hard-coded in the future if necessary
//  - perhaps some sort of "team" system could suffice
enum class AttackTarget {
    PLAYER,
    ENEMY
};

// ======================================
struct AttackHitInfo {
    uint32_t mTriggerFrame = std::numeric_limits<uint32_t>::max();

    float mOffsetX = 0.f;
    float mOffsetY = 0.f;
    float mWidth = 10.f;
    float mHeight = 10.f;

    float mDamage = 1.f;
    AttackTarget mTarget = AttackTarget::ENEMY;

    AttackHitInfo() = default;
    AttackHitInfo(uint32_t triggerFrame, float offsetX, float offsetY, float width, float height, float damage, AttackTarget target)
        : mTriggerFrame(triggerFrame), mOffsetX(offsetX), mOffsetY(offsetY), mWidth(width), mHeight(height), mDamage(damage), mTarget(target) {}
};

// ======================================
struct AttackInfo {
    std::string mAnimation;
    bool mDisableMovement = true;
    bool mInvulnerable = false;

    // TODO: This can be timed to exactly when the animation ends if desired
    float mDuration = 0.f;
    float mMoveSpeed = 0.f;

    std::vector<AttackHitInfo> mHitInfo;

    AttackInfo() = default;
    AttackInfo(const std::string& anim, bool disableMovement, bool invulnerable, float duration, float speed)
        : mAnimation(anim), mDisableMovement(disableMovement), mInvulnerable(invulnerable), mDuration(duration), mMoveSpeed(speed) {}
    AttackInfo(const std::string& anim, bool disableMovement, float duration, AttackHitInfo hitInfo)
        : mAnimation(anim), mDisableMovement(disableMovement), mDuration(duration), mHitInfo({ hitInfo }) {}
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
    
    bool TryDoAttack(const std::string& name);
    bool TriggerAttackHitEvent(const AttackHitInfo& attackHitInfo);

private:
    bool DoAttack(const std::string& name);

private:
    std::unordered_map<std::string, AttackInfo> mAttackMap;

    std::string mCurrentAttack;
    float mCurrentAttackTimer = 0.f;
    FacingDirection mCurrentAttackFacing;
    
};