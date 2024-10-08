#pragma once

#include "shade/game/entity/component/component.h"

#include "shade/file/keyValueFile.h"
#include "shade/game/entity/entity.h"
#include "shade/graphics/flare/flareService.h"

#include "components/facingComponent.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <limits>

// ======================================
// This could be generalized more instead of hard-coded in the future if necessary
//  - perhaps some sort of "team" system could suffice
enum class AttackTarget : int {
    PLAYER = 0,
    ENEMY,
    COUNT
};

// ======================================
struct AttackHitBox {
#ifdef BUILD_BREACH_EDITOR
    std::string mEffectPath;
#endif
    float mOffsetX = 0.f;
    float mOffsetY = 0.f;
    float mWidth = 10.f;
    float mHeight = 10.f;

    Shade::ResourceHandle mEffectTexture = Shade::ResourceHandle::Invalid;
    float mEffectOffsetX = 0.f;
    float mEffectOffsetY = 0.f;

    AttackHitBox() = default;
    AttackHitBox(float offsetX, float offsetY, float width, float height) : mOffsetX(offsetX), mOffsetY(offsetY), mWidth(width), mHeight(height) {}
    AttackHitBox(float offsetX, float offsetY, float width, float height, Shade::ResourceHandle effectTexture, float effectOffsetX, float effectOffsetY) : mOffsetX(offsetX), mOffsetY(offsetY), mWidth(width), mHeight(height), mEffectTexture(effectTexture), mEffectOffsetX(effectOffsetX), mEffectOffsetY(effectOffsetY) {}
};

// ======================================
struct AttackHitInfo {
#ifdef BUILD_BREACH_EDITOR
    void ShowImguiDetails();
    void SaveToKeyValueFile(Shade::KeyValueFile& file) const;
#endif
public:
    static AttackHitInfo LoadFromFileHandle(Shade::KeyValueHandle handle);

    uint32_t mTriggerFrame = std::numeric_limits<uint32_t>::max();
    float mDamage = 1.f;
    AttackTarget mTarget = AttackTarget::ENEMY;

    std::vector<AttackHitBox> mAttackBoxes;

    AttackHitInfo() = default;
    AttackHitInfo(uint32_t triggerFrame, float damage, AttackTarget target, std::vector<AttackHitBox>&& attackBoxes)
        : mTriggerFrame(triggerFrame), mDamage(damage), mTarget(target), mAttackBoxes(attackBoxes) {}
};

// ======================================
struct AttackInfo {
#ifdef BUILD_BREACH_EDITOR
    void ShowImguiDetails();
    void SaveToKeyValueFile(Shade::KeyValueFile& file) const;
#endif
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
    static const std::string ComponentID;
#ifdef BUILD_BREACH_EDITOR
public:
    virtual const char* GetComponentID() const { return ComponentID.c_str(); }
    virtual const char* GetDisplayName() const override { return "Attack Component"; }
    virtual void ShowImguiDetails() override;
    virtual void SaveToKeyValueFile(Shade::KeyValueFile& file) const override;
#endif
public:
    static AttackComponent* LoadFromFileHandle(Shade::KeyValueHandle handle);
public:
    AttackComponent();

    AttackInfo& RegisterAttackInfo(const std::string& name, const AttackInfo& attack);
    // This must be called after the animated sprite component has been set up
    void RegisterAttacksToAnimFrames();

    virtual void Initialize() override;
    virtual void Update(float deltaSeconds) override;

    bool IsDoingAttack() const;
    
    bool TryDoAttack(const std::string& name);
    void TriggerAttackHitEvent(const AttackHitInfo& attackHitInfo);

private:
    bool DoAttack(const std::string& name);

private:
    std::unordered_map<std::string, std::vector<AttackInfo>> mAttackMap;

    // In-engine attack state
    std::string mCurrentAttack;
    std::vector<Shade::Entity*> mCurrentHitEnemies; // Entity pointers are used directly for checks, no lifetime safety needed
    float mCurrentAttackTimer = 0.f;
    FacingDirection mCurrentAttackFacing;

    // Combo data
    size_t mCurrentAttackIndex = 0;
    float mComboWindow = 0.f;
    std::string mLastAttack;
};