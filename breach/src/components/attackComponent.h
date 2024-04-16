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
struct AttackInfo {
    float mOffsetX = 0.f;
    float mOffsetY = 0.f;
    float mWidth = 10.f;
    float mHeight = 10.f;

    float mDamage = 1.f;
    AttackTarget mTarget = AttackTarget::ENEMY;

    AttackInfo() = default;
    AttackInfo(float offsetX, float offsetY, float width, float height, float damage, AttackTarget target)
        : mOffsetX(offsetX), mOffsetY(offsetY), mWidth(width), mHeight(height), mDamage(damage), mTarget(target) {}
};

// ======================================
// Entity attacks are stored via a map with the name of the attack mapping to the attack data
class AttackComponent : public Shade::Component {
public:
    AttackComponent();

    void RegisterAttackInfo(const std::string& name, const AttackInfo& attack);
    // This must be called after the animated sprite component has been set up
    void RegisterAttacksToAnimFrames(const std::vector<std::pair<std::string, uint16_t>>& attackAnimData);

    bool DoAttack(const std::string& name);

private:
    std::unordered_map<std::string, AttackInfo> mAttackMap;
    
};